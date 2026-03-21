#ifndef __GRID_OBJECT_H__
#define __GRID_OBJECT_H__

#include "cocos2d.h"
#include "CCVec2i.h"

class PlayLayer;

class GridObject : public cocos2d::CCSprite {
public:
	CREATE_FUNC(GridObject);
	GridObject();

	bool init() override;
	void onEnter() override;

	virtual void setCell(const cocos2d::Vec2i& cell);
	const cocos2d::Vec2i& getCell() const;

	virtual void onPlayerOverlap();
protected:
	void snapToGrid(const cocos2d::Vec2i& cell);

	cocos2d::Vec2i m_cell;
	PlayLayer* m_playLayer;
};

#endif //!__GRID_OBJECT_H__