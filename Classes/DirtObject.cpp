#include "DirtObject.h"

bool DirtObject::init() {
	if (!GameObject::init()) {
		return false;
	}

	setTexture("CloseNormal.png");

	return true;
}