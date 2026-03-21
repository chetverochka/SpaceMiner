#include "PlayScene.h"
#include "PlayLayer.h"
#include "GridObject.h"

USING_NS_CC;

PlayScene::PlayScene() 
	
{}

bool PlayScene::init() {
	if (!Scene::init()) {
		return false;
	}


	PlayLayer* playLayer = PlayLayer::create();
	addChild(playLayer, 0);

	for (int i = 0; i < 10; i++) {
		GridObject* object = GridObject::create();

		Vec2i cell(10 + i, 10);
		object->setCell(cell);
		playLayer->addObject(object);
	}

	return true;
}

void PlayScene::generateSpawnObjects(cocos2d::Node* pln) {

}