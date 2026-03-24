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
	virtual void setGridStep(const cocos2d::Vec2& gridStep);
	virtual void setGridOffset(const cocos2d::Vec2& gridOffset);

	const cocos2d::Vec2i& getCell() const;

	void setPlayLayer(PlayLayer* playLayer);
	PlayLayer* getPlayLayer();
protected:
	void snapToGrid(const cocos2d::Vec2i& cell, const cocos2d::Vec2& gridStep, const cocos2d::Vec2& offset);

	cocos2d::Vec2i m_cell;
	cocos2d::Vec2 m_gridStep;
	cocos2d::Vec2 m_gridOffset;

	PlayLayer* m_playLayer;
};

#endif //!__GRID_OBJECT_H__