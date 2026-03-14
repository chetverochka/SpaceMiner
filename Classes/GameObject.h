#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"

class ObjectEventDelegate;

class GameObject : public cocos2d::CCSprite {
public:
	CREATE_FUNC(GameObject);

	GameObject();
private:
	ObjectEventDelegate* m_eventDelegate;
	std::pair<int, int> m_cell;
};

#endif //!__GAME_OBJECT_H__