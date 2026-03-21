#include "GridObject.h"
#include "PlayLayer.h"

USING_NS_CC;

GridObject::GridObject() 
	: m_cell(Vec2i(0,0))
	, m_playLayer(NULL)
{
}

bool GridObject::init() {
	if (!Sprite::init()) {
		return false;
	}

	setTexture("Stone.png");

	return true;
}
// TODO сделать отдельный метод для добавления в PlayLayer. !т.к. привязанный родитель не обязан быть PlayLayer!
void GridObject::onEnter() {
	Sprite::onEnter();
	m_playLayer = dynamic_cast<PlayLayer*>(_parent);
	CCAssert(m_playLayer, "GridObject::onEnter: parent != PlayLayer");

	snapToGrid(m_cell);
}

void GridObject::setCell(const Vec2i& cell) {
	if (cell != m_cell) {
		m_cell = cell;
		snapToGrid(cell);
	}
}

const Vec2i& GridObject::getCell() const {
	return m_cell;
}

void GridObject::onPlayerOverlap() {
	CCLOG("Player overlapping object!");
}

void GridObject::snapToGrid(const Vec2i& cell) {
	const Vec2 gridStep = m_playLayer->getGridStep();

	Vec2 position;
	position.x = gridStep.x / 2 + gridStep.x * cell.x;
	position.y = gridStep.y / 2 + gridStep.y * cell.y;

	setPosition(position);
}
