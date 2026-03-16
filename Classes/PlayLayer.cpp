#include "PlayLayer.h"
#include "GameObject.h"
#include "ChunkMap.h"
#include "PlayerObject.h"

USING_NS_CC;

PlayLayer::PlayLayer()
	: m_chunkMap(NULL) // cocos2d-x ruins RAII and i love it lol
{}

PlayLayer::~PlayLayer() {
	if (m_chunkMap) {
		delete m_chunkMap;
		m_chunkMap = NULL;
	}
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

	m_chunkMap = new ChunkMap();

	m_player = PlayerObject::create();
	addObject(m_player);

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

	for (int i = 0; i < m_allObjects.size(); i++) {
		GameObject* object = m_allObjects.at(i);
		bool selfUpdatable = object->getScheduler()->isScheduled(schedule_selector(GameObject::update), object);
		if (!selfUpdatable) {
			object->update(deltaTime);
		}
	}
}

void PlayLayer::addObject(GameObject* object) {
	if (m_allObjects.contains(object)) {
		return;
	}
	m_allObjects.pushBack(object);

	if (!object->getParent()) {
		addChild(object, 0);
	}
}

void PlayLayer::removeObject(GameObject* object) {
	if (!m_allObjects.contains(object)) {
		return;
	}
	m_allObjects.eraseObject(object);
	if (object->getParent() == this) {
		object->removeFromParent();
	}
}

void PlayLayer::ccKeyPressed(EventKeyboard::KeyCode key, Event* event) {
	event->stopPropagation();

	int targetMoveDirX = 0, targetMoveDirY = 0;

	typedef EventKeyboard::KeyCode Key;
	switch (key) {
	default:
		break;
	case Key::KEY_W:
		targetMoveDirY = 1;
		break;
	case Key::KEY_A:
		targetMoveDirX = -1;
		break;
	case Key::KEY_S:
		targetMoveDirY = -1;
		break;
	case Key::KEY_D:
		targetMoveDirX = 1;
		break;
	}

	m_player->smoothMove(targetMoveDirX, targetMoveDirY);

	CCVec2i playerCell(m_player->getCellX(), m_player->getCellY());
	CCVec2i chunkIndex(m_chunkMap->getChunkCoords(playerCell));
	CCLOG("Chunk coords for position (%i, %i) = %i, %i;", playerCell.x, playerCell.y, chunkIndex.x, chunkIndex.y);
}

void PlayLayer::ccKeyReleased(EventKeyboard::KeyCode key, Event* event) {
	event->stopPropagation();
	typedef EventKeyboard::KeyCode Key;

}