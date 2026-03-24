#include "PlayScene.h"
#include "PlayLayer.h"
#include "MineableObject.h"
#include "Matrix.h"

USING_NS_CC;

PlayScene::PlayScene() 
	
{}

bool PlayScene::init() {
	if (!Scene::init()) {
		return false;
	}


	PlayLayer* playLayer = PlayLayer::create();
	addChild(playLayer, 0);

	for (int i = 1; i <= 10; i++) {
		for (int j = 1; j <= 10; j++) {
			GridObject* object = MineableObject::create();

			Vec2i cell(3 + i, 3 + j);
			object->setCell(cell);
			playLayer->addObject(object);
		}
	}

	return true;
}

void PlayScene::generateSpawnObjects(cocos2d::Node* pln) {

}