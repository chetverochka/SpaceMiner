#ifndef __GRID_OBJECT_H__
#define __GRID_OBJECT_H__

#include "cocos2d.h"
#include "CCVec2i.h"

class PlayLayer;

class GridObject : public cocos2d::CCSprite {
public:
	enum OverlapResult {
		NONE,
		OBJECT_BREAKED
	};

	struct OverlapContext {
		int strength;

	};

	CREATE_FUNC(GridObject);
	GridObject();

	bool init() override;
	void onEnter() override;

	virtual void setCell(const cocos2d::Vec2i& cell);
	virtual void setGridStep(const cocos2d::Vec2& gridStep);
	virtual void setGridOffset(const cocos2d::Vec2& gridOffset);

	const cocos2d::Vec2i& getCell() const;

	virtual OverlapResult onPlayerOverlap(OverlapContext& context);
protected:
	void snapToGrid(const cocos2d::Vec2i& cell, const cocos2d::Vec2& gridStep, const cocos2d::Vec2& offset);

	cocos2d::Vec2i m_cell;
	cocos2d::Vec2 m_gridStep;
	cocos2d::Vec2 m_gridOffset;
};

#endif //!__GRID_OBJECT_H__