#include "MineableObject.h"
#include <algorithm>
#include "ui/CocosGUI.h"

USING_NS_CC;

const float MineableObject::BASE_OBJECT_SCALE = 0.9f;

MineableObject* MineableObject::createWithType(MineableObject::MineableType type) {
	typedef MineableObject::MineableType Type;

	MineableObject::CreateData createData;
	std::string frameName = "";
	ui::Widget::TextureResType resType = ui::Widget::TextureResType::LOCAL;

	switch (type) {
	default:
	case Type::DEFAULT:
		createData.maxStrength = 3;
		break;
	case Type::DIAMOND:
		createData.maxStrength = 7;
		break;
	case Type::GOLD:
		createData.maxStrength = 2;
		break;
	case Type::COPPER:
		createData.maxStrength = 4;
		break;
	case Type::OBSIDIAN:
		createData.maxStrength = 50;
		break;
	}

	MineableObject* object = MineableObject::create(createData);
	switch (resType) {
	default:
	case ui::Widget::TextureResType::LOCAL:
		object->setTexture(frameName);
		break;
	case ui::Widget::TextureResType::PLIST:
		object->setSpriteFrame(frameName);
		break;
	}

	return object;
}

MineableObject* MineableObject::create(CreateData createData) {
	MineableObject* object = new (std::nothrow) MineableObject();
	if (object && object->initWithValues(createData)) {
		object->autorelease();
		return object;
	}
	CC_SAFE_DELETE(object);
	return NULL;
}

MineableObject::MineableObject()

{}

bool MineableObject::initWithValues(MineableObject::CreateData createData) {
	if (!GridObject::init()) {
		return false;
	}

	m_maxStrength = createData.maxStrength;
	m_estimatedStrength = m_maxStrength;
	setScale(BASE_OBJECT_SCALE);



	return true;
}

bool MineableObject::init() {
	CreateData createData;
	createData.maxStrength = 3;
	return initWithValues(createData);
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

/*
void MineableObject::setMaxStrength(const int maxStrength) {
	m_maxStrength = maxStrength;
	resetMine();
}
*/

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
	float scale = (float)m_estimatedStrength / (float)m_maxStrength * BASE_OBJECT_SCALE;
	float rotate = RandomHelper::random_real<float>(10.f, 20.f); // 10.f perfect value
	if (RandomHelper::random_int<int>(0, 1)) {
		rotate = -rotate;
	}
	runAction(CCSpawn::create({
		CCEaseBackIn::create(CCScaleTo::create(0.1f, scale)),
		CCSequence::create({
			CCEaseElasticOut::create(CCRotateBy::create(0.02f, rotate)),
			CCEaseBackOut::create(CCRotateBy::create(0.02f, -rotate))
			})
		}));
}
