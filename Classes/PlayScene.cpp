#include "PlayScene.h"
#include "PlayLayer.h"
USING_NS_CC;

PlayScene::PlayScene() 
	
{}

bool PlayScene::init() {
	if (!Scene::init()) {
		return false;
	}

	PlayLayer* playLayer = PlayLayer::create();
	//playLayer->setPosition(Vec2(100, 100));
	addChild(playLayer, 0);

	return true;
}

void PlayScene::generateSpawnObjects(cocos2d::Node* pln) {

}