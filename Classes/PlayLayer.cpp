#include "PlayLayer.h"
#include "GameObject.h"
#include "ChunkMap.h"
#include "PlayerObject.h"

USING_NS_CC;

PlayLayer::PlayLayer()
	: m_chunkMap(NULL) // cocos2d-x ruins RAII and i love it lol
	, m_drawCanvas(NULL)
	, m_gridStep(Vec2(40,40))
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

	m_drawCanvas = DrawNode::create();
	if (!m_drawCanvas) {
		return false;
	}
	addChild(m_drawCanvas, 0, "draw_canvas");

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

void PlayLayer::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) {
	Layer::draw(renderer, transform, flags);

	m_drawChunksCommand.init(getGlobalZOrder(), transform, flags);
	m_drawChunksCommand.func = CC_CALLBACK_0(PlayLayer::drawChunks, this);

	renderer->addCommand(&m_drawChunksCommand);
}

void PlayLayer::onObjectCellMoved(GameObject* target, int destinationX, int destinationY) {
	m_chunkMap->updateObject(target);
}


void PlayLayer::addObject(GameObject* object) {
	if (m_allObjects.contains(object)) {
		return;
	}
	m_allObjects.pushBack(object);

	object->addEventListener(this);

	if (!object->getParent()) {
		addChild(object, 0);
	}
	m_chunkMap->add(object);
}

void PlayLayer::removeObject(GameObject* object) {
	if (!m_allObjects.contains(object)) {
		return;
	}
	m_allObjects.eraseObject(object);

	object->removeEventListener();

	if (object->getParent() == this) {
		object->removeFromParent();
	}
	m_chunkMap->remove(object);
}

void PlayLayer::drawChunks() {
	if (!m_drawCanvas) {
		return;
	}

	m_drawCanvas->clear();
	
	std::vector<CCVec2i> chunks = m_chunkMap->getChunks();
	for (CCVec2i& chunkPos : chunks) {
		Vec2 chunkSize;
		chunkSize.x = m_chunkMap->getCellsCount().x * m_gridStep.x;
		chunkSize.y = m_chunkMap->getCellsCount().y * m_gridStep.y;

		Vec2 origin;
		origin.x = chunkPos.x * chunkSize.x;
		origin.y = chunkPos.y * chunkSize.y;

		Vec2 destination = origin + chunkSize;

		m_drawCanvas->drawRect(origin, destination, Color4F::WHITE);

		Color4F lineColor = Color4F(1, 1, 1, 0.3);

		// vertical lines
		for (int j = 0; j < m_chunkMap->getCellsCount().x; j++) {
			Vec2 from, to;

			from.x = origin.x + j * m_gridStep.x + m_gridStep.x;
			from.y = origin.y;

			to.x = from.x;
			to.y = destination.y;

			m_drawCanvas->drawLine(from, to, lineColor);
		}

		// horizontal lines
		for (int j = 0; j < m_chunkMap->getCellsCount().y; j++) {
			Vec2 from, to;

			from.x = origin.x;
			from.y = origin.y + j * m_gridStep.y + m_gridStep.y;

			to.x = destination.x;
			to.y = from.y;

			m_drawCanvas->drawLine(from, to, lineColor);
		}
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

	if (targetMoveDirX == 0 && targetMoveDirY == 0) {
		Vector<GameObject*>::iterator it = m_allObjects.end() - 1;
		while (dynamic_cast<PlayerObject*>(*it) && it != m_allObjects.begin()) {
			it--;
		}

		if (!dynamic_cast<PlayerObject*>(*it)) {
			removeObject(*it);
		}
	}
}

void PlayLayer::ccKeyReleased(EventKeyboard::KeyCode key, Event* event) {
	event->stopPropagation();
	typedef EventKeyboard::KeyCode Key;

}