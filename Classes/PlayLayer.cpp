#include "PlayLayer.h"
#include "GameObject.h"
#include "ChunkMap.h"
#include "PlayerObject.h"

USING_NS_CC;

PlayLayer::PlayLayer()

{}

bool PlayLayer::init(){
	if (!Layer::init()) {
		return false;
	}

	EventListenerKeyboard* keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayLayer::ccKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayLayer::ccKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

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
	typedef EventKeyboard::KeyCode Key;
	switch (key) {
	default:
		break;
	case Key::KEY_W:
		m_player->smoothMove(0, 1);
		break;
	case Key::KEY_A:
		m_player->smoothMove(-1, 0);
		break;
	case Key::KEY_S:
		m_player->smoothMove(0, -1);
		break;
	case Key::KEY_D:
		m_player->smoothMove(1, 0);
		break;
	}
}

void PlayLayer::ccKeyReleased(EventKeyboard::KeyCode key, Event* event) {
	event->stopPropagation();
	typedef EventKeyboard::KeyCode Key;

}