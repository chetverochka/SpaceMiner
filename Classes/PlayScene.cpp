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
	addChild(playLayer, 0);
	
	playLayer->addObject(DirtObject::create());
	playLayer->addObject(DirtObject::create());

	return true;
}