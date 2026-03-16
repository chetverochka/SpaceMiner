#ifndef __PLAYER_OBJECT_H__
#define __PLAYER_OBJECT_H__

#include "GameObject.h"

class PlayerObject : public GameObject {
public:
	CREATE_FUNC(PlayerObject);

	bool init() override;

	void smoothMove(const int cellX, const int cellY);
};

#endif //!__PLAYER_OBJECT_H__