#include "PlayScene.h"
#include "PlayLayer.h"
#include "DirtObject.h"
USING_NS_CC;

PlayScene::PlayScene() 
	
{}

bool PlayScene::init() {
	if (!Scene::init()) {
		return false;
	}

	PlayLayer* playLayer = PlayLayer::create();
	playLayer->setPosition(Vec2(100, 100));
	addChild(playLayer, 0);
	
	playLayer->addObject(DirtObject::create());
	playLayer->addObject(DirtObject::create());

	DirtObject* dirt = DirtObject::create();
	playLayer->addObject(dirt);
	dirt->setCell(26, 10);


	generateSpawnObjects(playLayer);

	return true;
}

void PlayScene::generateSpawnObjects(cocos2d::Node* pln) {
	PlayLayer* pl = static_cast<PlayLayer*>(pln);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			int x = j, y = i;

			GameObject* object = DirtObject::create();
			
			object->setCell(x, y);
			pl->addObject(object);
			
		}
	}
}