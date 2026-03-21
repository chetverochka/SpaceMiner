#include "GridObject.h"

USING_NS_CC;

GridObject::GridObject() 
	: m_cell(Vec2i(0,0))
{
}

bool GridObject::init() {
	if (!Sprite::init()) {
		return false;
	}

	return true;
}

void GridObject::onEnter() {
	Sprite::onEnter();
	updateGridPosition();
}

void GridObject::updateGridPosition() {
	// здесь надо как-то получать gridstep из PlayLayer 
}