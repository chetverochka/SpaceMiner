#include "GridObject.h"
#include "PlayLayer.h"

USING_NS_CC;

GridObject::GridObject() 
	: m_cell(Vec2i(0,0))
	//, m_playLayer(NULL)
{
}

bool GridObject::init() {
	if (!Sprite::init()) {
		return false;
	}

	setTexture("Stone.png");

	return true;
}

void GridObject::onEnter() {
	Sprite::onEnter();
	//m_playLayer = dynamic_cast<PlayLayer*>(_parent);
	//CCAssert(m_playLayer, "GridObject::onEnter: parent != PlayLayer");

	snapToGrid(m_cell, m_gridStep, m_gridOffset);
}

void GridObject::setCell(const Vec2i& cell) {
	if (cell != m_cell) {
		m_cell = cell;
		snapToGrid(m_cell, m_gridStep, m_gridOffset);
	}
}

void GridObject::setGridStep(const cocos2d::Vec2& gridStep) {
	m_gridStep = gridStep;
	snapToGrid(m_cell, gridStep, m_gridOffset);
}

void GridObject::setGridOffset(const cocos2d::Vec2& gridOffset) {
	m_gridOffset = gridOffset;
	snapToGrid(m_cell, m_gridStep, gridOffset);
}

const Vec2i& GridObject::getCell() const {
	return m_cell;
}

void GridObject::snapToGrid(const Vec2i& cell, const Vec2& gridStep, const Vec2& offset) {
	Vec2 position;
	position.x = gridStep.x / 2 + gridStep.x * cell.x + offset.x;
	position.y = gridStep.y / 2 + gridStep.y * cell.y + offset.y;

	setPosition(position);
}
