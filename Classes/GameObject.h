#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"

class ObjectEventDelegate;

class GameObject : public cocos2d::CCSprite {
public:
	static ObjectEventDelegate ms_safeEventDelegate;
	CREATE_FUNC(GameObject);

	GameObject();
private:
	ObjectEventDelegate* m_eventDelegate;
	int m_cellX, m_cellY;
};

#endif //!__GAME_OBJECT_H__