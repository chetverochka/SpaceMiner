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