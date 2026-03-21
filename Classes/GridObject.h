#ifndef __GRID_OBJECT_H__
#define __GRID_OBJECT_H__

#include "cocos2d.h"
#include "CCVec2i.h"

class GridObject : public cocos2d::CCSprite {
public:
	CREATE_FUNC(GridObject);
	GridObject();

	bool init() override;
	void onEnter() override;
protected:
	void updateGridPosition();

	cocos2d::Vec2i m_cell;
};

#endif //!__GRID_OBJECT_H__