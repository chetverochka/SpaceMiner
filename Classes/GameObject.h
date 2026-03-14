#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"

class GameObject : public cocos2d::CCSprite {
public:
private:
	std::pair<int, int> m_cell;
};

#endif //!__GAME_OBJECT_H__