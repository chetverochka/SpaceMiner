#include "PlayerObject.h"

bool PlayerObject::init() {
	if (!GameObject::init()) {
		return false;
	}

	setTexture("CloseSelected.png");
	
	return true;
}

void PlayerObject::smoothMove(const int cellX, const int cellY) {
	setCellX(getCellX() + cellX);
	setCellY(getCellY() + cellY);
}