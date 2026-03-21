#include "MineableBlock.h"
#include <algorithm>

USING_NS_CC;

MineableBlock::MineableBlock()

{}

bool MineableBlock::init() {
	if (!GridObject::init()) {
		return false;
	}

	m_maxStrength = 23;
	m_estimatedStrength = m_maxStrength;

	return true;
}

GridObject::OverlapResult MineableBlock::onPlayerOverlap(GridObject::OverlapContext& context) {
	m_estimatedStrength -= context.strength;
	if (m_estimatedStrength < 0)
		m_estimatedStrength = 0;

	float scale = (float)m_estimatedStrength / (float)m_maxStrength;

	runAction(CCEaseBackIn::create(CCScaleTo::create(0.1f, scale)));

	if (scale <= 0.f) {
		return GridObject::OverlapResult::OBJECT_BREAKED;
	}
	return GridObject::OverlapResult::NONE;
}
