#include "PlayLayer.h"
#include "GridObject.h"
#include "MineableObject.h"

USING_NS_CC;

const int PlayLayer::PLAYER_MOVE_ACTION_TAG = 1;
const int PlayLayer::PLAYER_MOVE_ROTATE_ACTION_TAG = 2;
const Vec2i PlayLayer::CHUNK_SIZE = Vec2i(4, 4);

PlayLayer::PlayLayer()
	: m_drawCanvas(NULL)
	, m_playerCell(Vec2i(0, 0))
	, m_playerSprite(NULL)
	, m_blockAimSprite(NULL)
	, m_allObjects(Vector<GridObject*>())
{
	m_allObjects.reserve(2000);
}

PlayLayer::~PlayLayer() {

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

	return true;
}

void PlayLayer::onEnter() {
	Layer::onEnter();
	scheduleUpdate();
}

void PlayLayer::onExit() {
	Layer::onExit();
	unscheduleUpdate();
}

void PlayLayer::update(float deltaTime) {
	Layer::update(deltaTime);

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
		const Size chunkSizePx(gridStep.x * CHUNK_SIZE.x, gridStep.y * CHUNK_SIZE.y);
		const Vec2 chunkOffsetPx(0, 0); // for future
		std::vector<Vec2i> activeChunks = getActiveChunks();

		for (int i = 0; i < activeChunks.size(); i++) {
			{
				Vec2 from, to;

				from.x = chunkSizePx.width * activeChunks[i].x + chunkOffsetPx.x;
				from.y = chunkSizePx.height * activeChunks[i].y + chunkOffsetPx.y;

				to.x = from.x + chunkSizePx.width;
				to.y = from.y + chunkSizePx.height;

				m_drawCanvas->drawRect(from, to, Color4F::BLUE);
				m_drawCanvas->drawSolidRect(from, to, Color4F(0, 0, 1, 0.2f));
			}

			// vertical lines
			for (int j = 0; j < CHUNK_SIZE.x - 1; j++) {
				Vec2 from, to;
				from.x = j * gridStep.x + gridStep.x + activeChunks[i].x * chunkSizePx.width;
				from.y = activeChunks[i].y * chunkSizePx.height;

				to.x = from.x;
				to.y = from.y + chunkSizePx.height;

				m_drawCanvas->drawLine(from, to, Color4F(0, 0, 1, 0.2f));
			}

			// horizontal lines
			for (int j = 0; j < CHUNK_SIZE.y - 1; j++) {
				Vec2 from, to;
				from.x = activeChunks[i].x * chunkSizePx.width;
				from.y = activeChunks[i].y * chunkSizePx.height + j * gridStep.y + gridStep.y;

				to.x = from.x + chunkSizePx.width;
				to.y = from.y;

				m_drawCanvas->drawLine(from, to, Color4F(0, 0, 1, 0.2f));
			}
		}
	}

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

Vec2i PlayLayer::computeChunkForObject(GridObject* object) {
	return computeChunkPos(object->getCell());
}


Vec2i PlayLayer::computeChunkPos(const cocos2d::Vec2i& globalCell) {
	Vec2i ret;
	ret.x = (int)std::floorf((float) globalCell.x / CHUNK_SIZE.x );
	ret.y = (int)std::floorf((float) globalCell.y / CHUNK_SIZE.y );
	return ret;
}
