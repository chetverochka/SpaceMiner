#include "PlayLayer.h"
#include "GridObject.h"
#include "MineableObject.h"
USING_NS_CC;

const int PlayLayer::PLAYER_MOVE_ACTION_TAG = 1;
const int PlayLayer::PLAYER_MOVE_ROTATE_ACTION_TAG = 2;
const Vec2i PlayLayer::CHUNK_SIZE = Vec2i(7,7);

PlayLayer::PlayLayer()
	: m_drawCanvas(NULL)
	, m_playerCell(Vec2i(0, 0))
	, m_playerSprite(NULL)
	, m_blockAimSprite(NULL)
	, m_allObjects(Vector<GridObject*>())
	, m_camera(NULL)
	, m_debugDrawMode(true)
{
	m_allObjects.reserve(2000);
}

PlayLayer::~PlayLayer() {
	m_camera->release();
}

bool PlayLayer::init(){
	if (!Layer::init()) {
		return false;
	}

	// listeners block
	EventListenerKeyboard* keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayLayer::ccKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayLayer::ccKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	// end listeners block

	m_drawCanvas = DrawNode::create();
	if (!m_drawCanvas) {
		return false;
	}
	addChild(m_drawCanvas, 0, "draw_canvas");

	m_playerSprite = Sprite::create("PlayerSprite.png");
	addChild(m_playerSprite, 0);

	m_blockAimSprite = Sprite::create("BlockAim.png");
	m_blockAimSprite->setVisible(false);
	addChild(m_blockAimSprite, 0);

	const Size visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	m_camera = Camera::createOrthographic(visibleSize.width, visibleSize.height, -1024, 1024);
	if (!m_camera) {
		return false;
	}
	m_camera->retain();
	addChild(m_camera);

	m_visibleArea.size = visibleSize;
	m_visibleArea.origin = ccp(0, 0);

	return true;
}

void PlayLayer::onEnter() {
	Layer::onEnter();
	scheduleUpdate();

	Scene* scene = getScene();

}

void PlayLayer::onExit() {
	Layer::onExit();
	unscheduleUpdate();
}

void PlayLayer::update(float deltaTime) {
	Layer::update(deltaTime);

	if (isRunning()) {
		updateCameraPosition(deltaTime);
		generateChunksInArea(m_visibleArea);
	}
}

void PlayLayer::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) {
	Layer::draw(renderer, transform, flags);

	CustomCommand* drawCommand = &m_drawCommand;
	drawCommand->init(_globalZOrder, transform, flags);
	drawCommand->func = CC_CALLBACK_0(PlayLayer::debugDraw, this);
	renderer->addCommand(drawCommand);
}

cocos2d::Vec2 PlayLayer::getGridStep() {
	static const Vec2 gridStep(60.f, 60.f);
	return gridStep;
}

void PlayLayer::addObject(GridObject* object) {
	CCAssert(!m_allObjects.contains(object), "PlayLayer::addObject: can't add attached object twice!");
	m_allObjects.pushBack(object);

	object->setGridStep(getGridStep());
	object->setGridOffset(ccp(0, 0));

	m_chunkToObject[object] = computeChunkForObject(object);

	addChild(object, -1);
}

void PlayLayer::removeObject(GridObject* object) {
	CCAssert(m_allObjects.contains(object), "PlayLayer::addObject: can't remove unattached object!");
	m_allObjects.eraseObject(object);
	removeChild(object);

	m_chunkToObject.erase(object);
}

const cocos2d::Vector<GridObject*>& PlayLayer::getObjects() {
	return m_allObjects;
}

bool PlayLayer::isCellOccupied(const cocos2d::Vec2i& cell) const {
	return std::find_if(m_allObjects.begin(), m_allObjects.end(), 
		[cell](const GridObject* object) {
			return object->getCell() == cell;
		}
	) != m_allObjects.end();
}

GridObject* PlayLayer::getObjectInCell(const cocos2d::Vec2i& cell) {
	Vector<GridObject*>::iterator it = std::find_if(m_allObjects.begin(), m_allObjects.end(), [cell](GridObject* object) {
		return object->getCell() == cell;
		});
	return it == m_allObjects.end() ? NULL : *it;
}

void PlayLayer::updateObjectChunk(GridObject* object) {
	if (!m_allObjects.contains(object) || m_chunkToObject.count(object) < 1)
		return;

	m_chunkToObject[object] = computeChunkForObject(object);
}

std::vector<Vec2i> PlayLayer::getActiveChunks() const {
	std::vector<Vec2i> ret;

	for (auto& i = m_chunkToObject.begin(); i != m_chunkToObject.end(); i++) {
		Vec2i chunk = i->second;
		if (std::find_if(ret.begin(), ret.end(), [chunk](Vec2i& c) {
			return chunk == c;
			}) == ret.end() ) {
			ret.push_back(chunk);
		}
	}

	return ret;
}


void PlayLayer::debugDraw() {
	m_drawCanvas->clear();

	if (!m_debugDrawMode)
		return;

	const Vec2i playerCell = m_playerCell;
	const Vec2 gridStep = getGridStep();

	// Player Cell draw
	{
		Vec2 from, to;
		from = Vec2(gridStep.x * playerCell.x, gridStep.y * playerCell.y);
		to = from + gridStep;

		m_drawCanvas->drawRect(from, to, Color4F::WHITE);
		m_drawCanvas->drawSolidRect(from, to, Color4F(1, 1, 1, 0.2f));
	}

	// Chunks draw
	{
		std::vector<Vec2i> activeChunks = getActiveChunks();
		for (int i = 0; i < activeChunks.size(); i++) {
			debugDrawChunk(m_drawCanvas, Color4F::BLUE, ccp(0, 0), activeChunks[i].x, activeChunks[i].y);
		}
	}

	const Vec2i playerChunk = computeChunkPos(m_playerCell);
	debugDrawChunk(m_drawCanvas, Color4F::YELLOW, ccp(0, 0), playerChunk.x, playerChunk.y);


}

typedef EventKeyboard::KeyCode CCKey;

void PlayLayer::ccKeyPressed(CCKey key, Event* event) {
	event->stopPropagation();

	int targetMoveDirX = 0, targetMoveDirY = 0, targetRotation = 0;
	switch (key) {
	default:
		break;
	case CCKey::KEY_W:
		targetMoveDirY = 1;
		m_playerSprite->setRotation(0);
		break;
	case CCKey::KEY_A:
		targetMoveDirX = -1;
		m_playerSprite->setRotation(-90);
		break;
	case CCKey::KEY_S:
		targetMoveDirY = -1;
		targetRotation = -180;
		m_playerSprite->setRotation(-180);
		break;
	case CCKey::KEY_D:
		targetMoveDirX = 1;
		m_playerSprite->setRotation(90);
		break;
	case CCKey::KEY_G:
		m_debugDrawMode = !m_debugDrawMode;
		break;
	}

	if (targetMoveDirX != 0 || targetMoveDirY != 0) {
		const int newCellX = targetMoveDirX + m_playerCell.x;
		const int newCellY = targetMoveDirY + m_playerCell.y;

		const bool cellOccupied = isCellOccupied(Vec2i(newCellX, newCellY));

		const Vec2 gridStep = getGridStep();

		Vec2 absolutePosition;
		absolutePosition.x = getGridStep().x * newCellX + gridStep.x / 2;
		absolutePosition.y = getGridStep().y * newCellY + gridStep.y / 2;

		if (cellOccupied) {
			const int aimActionTag = 1;
			const float aimDuration = 0.2f;
			m_blockAimSprite->stopActionByTag(aimActionTag);

			CCAction* action = CCSequence::create({
				CCPlace::create(Vec2(m_playerSprite->getPosition())),
				CCFadeOut::create(0.f),
				CCScaleTo::create(0.f, 0.f),
				CCShow::create(),
				CCSpawn::create({
						EaseCubicActionInOut::create(MoveTo::create(aimDuration, absolutePosition)),
						CCFadeIn::create(aimDuration),
						CCEaseElasticInOut::create(CCScaleTo::create(aimDuration, 1.f)),
					}),
				CCDelayTime::create(0.2f), // 2.f
				CCSpawn::create({
						CCFadeOut::create(aimDuration),
						CCEaseBackIn::create(CCScaleTo::create(aimDuration, 0.f)),
					}),
				CCHide::create()
				});
			action->setTag(aimActionTag);

			m_blockAimSprite->runAction(action);

			MineableObject* mineableObject = dynamic_cast<MineableObject*>(getObjectInCell(Vec2i(newCellX, newCellY)));
			if (mineableObject) {
				mineableObject->mine(1);
				if (mineableObject->isBroken()) {
					removeObject(mineableObject);
				}
			}
		}
		else {
			m_playerCell = Vec2i(newCellX, newCellY);
			m_playerSprite->stopActionByTag(PLAYER_MOVE_ACTION_TAG);
			ActionInterval* moveAction = EaseBackInOut::create(MoveTo::create(0.1f, absolutePosition));
			moveAction->setTag(PLAYER_MOVE_ACTION_TAG);

			m_playerSprite->runAction(moveAction);
		}

		m_playerSprite->runAction(CCSequence::create({
				CCScaleTo::create(0.06f, 1.2f),
				CCScaleTo::create(0.06f, 1.0f)
			})
		);
	}

}

void PlayLayer::ccKeyReleased(CCKey key, Event* event) {
	event->stopPropagation();

}

void PlayLayer::updateCameraPosition(float deltaTime) {
	const float zoom = 1.f;
	const Size size = CCDirector::sharedDirector()->getVisibleSize() * zoom;
	const Vec2 center = m_playerSprite->getPosition();

	Vec2 targetPosition;
	targetPosition.x = center.x - size.width / 2;
	targetPosition.y = center.y - size.height / 2;
	m_camera->setPosition(targetPosition);

	m_visibleArea.size = size;
	m_visibleArea.origin = Vec2(center.x - size.width / 2, center.y - size.height / 2);
}

void PlayLayer::generateChunksInArea(cocos2d::Rect area) {
	Vec2i from, to;
	from = computeChunkPos(area.origin);
	to = computeChunkPos(Vec2(area.getMaxX(), area.getMaxY()));

	for (int chunkY = from.y; chunkY <= to.y; chunkY++) {
		for (int chunkX = from.x; chunkX <= to.x; chunkX++) {
			bool wasGenerated = false;

			for (int i = 0; i < m_generatedChunks.size(); i++) {
				if (m_generatedChunks[i] == Vec2i(chunkX, chunkY)) {
					wasGenerated = true;
					break;
				}
			}

			if (wasGenerated)
				continue;

			Vec2i fromCell, toCell;
			fromCell.x = chunkX * CHUNK_SIZE.x;
			fromCell.y = chunkY * CHUNK_SIZE.y;

			toCell.x = fromCell.x + CHUNK_SIZE.x;
			toCell.y = fromCell.y + CHUNK_SIZE.y;

			for (int cellY = fromCell.y; cellY <= toCell.y; cellY++) {
				for (int cellX = fromCell.x; cellX <= toCell.x; cellX++) {
					if (cellX > -3 && cellX < 3 && cellY > -3 && cellY < 3)
						continue;

					GridObject* object = MineableObject::create();
					
					object->setCell(Vec2i(cellX, cellY));
					addObject(object);

				}
			}

			m_generatedChunks.push_back(Vec2i(chunkX, chunkY));
		}
	}
}

void PlayLayer::debugDrawChunk(DrawNode* canvas, const Color4F& color, const Vec2& offSet, const int x, const int y) {
	const Vec2 gridStep = getGridStep();
	const Size chunkSizePx(gridStep.x * CHUNK_SIZE.x, gridStep.y * CHUNK_SIZE.y);
	const Color4F fillColor = Color4F(color.r, color.g, color.b, 0.2f);

	Vec2 from, to;

	from.x = chunkSizePx.width * x + offSet.x;
	from.y = chunkSizePx.height * y + offSet.y;

	to.x = from.x + chunkSizePx.width;
	to.y = from.y + chunkSizePx.height;

	canvas->drawRect(from, to, color);
	canvas->drawSolidRect(from, to, fillColor);

	// vertical lines
	for (int j = 0; j < CHUNK_SIZE.x - 1; j++) {
		Vec2 from, to;
		from.x = j * gridStep.x + gridStep.x + x * chunkSizePx.width;
		from.y = y * chunkSizePx.height;

		to.x = from.x;
		to.y = from.y + chunkSizePx.height;

		canvas->drawLine(from, to, fillColor);
	}

	// horizontal lines
	for (int j = 0; j < CHUNK_SIZE.y - 1; j++) {
		Vec2 from, to;
		from.x = x * chunkSizePx.width;
		from.y = y * chunkSizePx.height + j * gridStep.y + gridStep.y;

		to.x = from.x + chunkSizePx.width;
		to.y = from.y;

		canvas->drawLine(from, to, fillColor);
	}
}


Vec2i PlayLayer::computeChunkForObject(GridObject* object) {
	return computeChunkPos(object->getCell());
}


Vec2i PlayLayer::computeChunkPos(const Vec2i& globalCell) {
	Vec2i ret;
	ret.x = (int)std::floorf((float) globalCell.x / CHUNK_SIZE.x );
	ret.y = (int)std::floorf((float) globalCell.y / CHUNK_SIZE.y );
	return ret;
}

Vec2i PlayLayer::computeChunkPos(const Vec2& positionPx) {
	const Vec2 gridStep = getGridStep();
	const int cellX = std::floorf(positionPx.x / gridStep.x);
	const int cellY = std::floorf(positionPx.y / gridStep.y);
	return computeChunkPos(Vec2i(cellX, cellY));
}
