#include "PlayLayer.h"

USING_NS_CC;

PlayLayer::PlayLayer()

{}

bool PlayLayer::init(){
	if (!Layer::init()) {
		return false;
	}

	return true;
}