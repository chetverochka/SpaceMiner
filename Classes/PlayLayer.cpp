#include "PlayLayer.h"

USING_NS_CC;

const int PlayLayer::PLAYER_MOVE_ACTION_TAG = 1;

PlayLayer::PlayLayer()
	: m_drawCanvas(NULL)
	, m_playerCell(Vec2i(0, 0))
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

void PlayLayer::debugDraw() {
	m_drawCanvas->clear();

	const Vec2i playerCell = m_playerCell;
	const Vec2 gridStep = getGridStep();

	Vec2 from, to;
	from = Vec2(gridStep.x * playerCell.x, gridStep.y * playerCell.y);
	to = from + gridStep;

	m_drawCanvas->drawRect(from, to, Color4F::WHITE);
	m_drawCanvas->drawSolidRect(from, to, Color4F(1, 1, 1, 0.2f));
}

typedef EventKeyboard::KeyCode CCKey;

void PlayLayer::ccKeyPressed(CCKey key, Event* event) {
	event->stopPropagation();

	int targetMoveDirX = 0, targetMoveDirY = 0;
	switch (key) {
	default:
		break;
	case CCKey::KEY_W:
		targetMoveDirY = 1;
		break;
	case CCKey::KEY_A:
		targetMoveDirX = -1;
		break;
	case CCKey::KEY_S:
		targetMoveDirY = -1;
		break;
	case CCKey::KEY_D:
		targetMoveDirX = 1;
		break;
	}

	const int newCellX = targetMoveDirX + m_playerCell.x;
	const int newCellY = targetMoveDirY + m_playerCell.y;

	const bool cellOccupied = false;

	if (!cellOccupied) {
		m_playerCell = Vec2i(newCellX, newCellY);

		const Vec2 gridStep = getGridStep();

		Vec2 newPlayerPosition;
		newPlayerPosition.x = getGridStep().x * newCellX + gridStep.x / 2;
		newPlayerPosition.y = getGridStep().y * newCellY + gridStep.y / 2;

		m_playerSprite->stopActionByTag(PLAYER_MOVE_ACTION_TAG);

		ActionInterval* moveAction = EaseBackInOut::create(MoveTo::create(0.1f, newPlayerPosition));
		moveAction->setTag(PLAYER_MOVE_ACTION_TAG);

		m_playerSprite->runAction(moveAction);
	}
}

void PlayLayer::ccKeyReleased(CCKey key, Event* event) {
	event->stopPropagation();

}