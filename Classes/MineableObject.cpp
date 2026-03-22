#include "MineableObject.h"
#include <algorithm>

USING_NS_CC;

MineableObject::MineableObject()

{}

bool MineableObject::init() {
	if (!GridObject::init()) {
		return false;
	}

	m_maxStrength = 4;
	m_estimatedStrength = m_maxStrength;

	setScale(1.f - 0.1f);

	return true;
}

//GridObject::OverlapResult MineableBlock::onPlayerOverlap(GridObject::OverlapContext& context) {
//	m_estimatedStrength -= context.strength;
//	if (m_estimatedStrength < 0)
//		m_estimatedStrength = 0;
//
//	float scale = (float)m_estimatedStrength / (float)m_maxStrength - 0.1f;
//
//	runAction(CCEaseBackIn::create(CCScaleTo::create(0.1f, scale)));
//
//	if (m_estimatedStrength <= 0) {
//		return GridObject::OverlapResult::OBJECT_BREAKED;
//	}
//	return GridObject::OverlapResult::NONE;
//}

void MineableObject::setMaxStrength(const int maxStrength) {
	m_maxStrength = maxStrength;
	resetMine();
}

int MineableObject::getMaxStrength() const {
	return m_maxStrength;
}
int MineableObject::getEstimatedStrength() const {
	return m_estimatedStrength;
}

void MineableObject::mine(const int strength) {
	m_estimatedStrength -= strength;
	if (m_estimatedStrength < 0)
		m_estimatedStrength = 0;
	else if (m_estimatedStrength > m_maxStrength)
		m_estimatedStrength = m_maxStrength;

	onMiningInteraction(m_estimatedStrength, m_maxStrength);
}
void MineableObject::resetMine() {
	m_estimatedStrength = m_maxStrength;
}

bool MineableObject::isBroken() {
	return m_estimatedStrength <= 0;
}

void MineableObject::onMiningInteraction(const int currentStrength, const int maxStrength) {
	float scale = (float)m_estimatedStrength / (float)m_maxStrength - 0.1f;
	runAction(CCEaseBackIn::create(CCScaleTo::create(0.1f, scale)));
}
