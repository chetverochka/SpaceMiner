#include "PlayLayer.h"
#include "GridObject.h"

USING_NS_CC;

const int PlayLayer::PLAYER_MOVE_ACTION_TAG = 1;
const int PlayLayer::PLAYER_MOVE_ROTATE_ACTION_TAG = 2;

PlayLayer::PlayLayer()
	: m_drawCanvas(NULL)
	, m_playerCell(Vec2i(0, 0))
	, m_playerSprite(NULL)
	, m_blockAimSprite(NULL)
	, m_allObjects(Vector<GridObject*>())
{}

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

	addChild(object, -1);
}

void PlayLayer::removeObject(GridObject* object) {
	CCAssert(m_allObjects.contains(object), "PlayLayer::addObject: can't remove unattached object!");
	m_allObjects.eraseObject(object);
	removeChild(object);
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


void PlayLayer::debugDraw() {
	m_drawCanvas->clear();

	const Vec2i playerCell = m_playerCell;
	const Vec2 gridStep = getGridStep();

	Vec2 from, to;
	from = Vec2(gridStep.x * playerCell.x, gridStep.y * playerCell.y);
	to = from + gridStep;

	//m_drawCanvas->drawRect(from, to, Color4F::WHITE);
	//m_drawCanvas->drawSolidRect(from, to, Color4F(1, 1, 1, 0.2f));
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
				CCDelayTime::create(2.f),
				CCSpawn::create({
						CCFadeOut::create(aimDuration),
						CCEaseBackIn::create(CCScaleTo::create(aimDuration, 0.f)),
					}),
				CCHide::create()
				});
			action->setTag(aimActionTag);

			m_blockAimSprite->runAction(action);

			GridObject* overlapObject = getObjectInCell(Vec2i(newCellX, newCellY));
			if (overlapObject) {
				GridObject::OverlapContext overlapCtx;
				overlapCtx.strength = 1;

				const GridObject::OverlapResult result = overlapObject->onPlayerOverlap(overlapCtx);
				if (result == GridObject::OverlapResult::OBJECT_BREAKED) {
					removeObject(overlapObject);
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