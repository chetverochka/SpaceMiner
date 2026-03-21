#include "CCVec2i.h"

using namespace cocos2d;

Vec2i::Vec2i() : x(0), y(0) {}
Vec2i::Vec2i(int x, int y) : x(x), y(y) {}
Vec2i::Vec2i(const cocos2d::CCPoint& vec2f) : x((int)vec2f.x), y((int)vec2f.y) {}
Vec2i::Vec2i(const cocos2d::CCSize& size) : x((int)size.width), y((int)size.height) {}
Vec2i::Vec2i(int xy) : x(xy), y(xy) {}
Vec2i::Vec2i(const Vec2i& copyObj) : x(copyObj.x), y(copyObj.y) {}

bool Vec2i::operator==(const Vec2i& comparable) const {
	return x == comparable.x && y == comparable.y;
}

bool Vec2i::operator!=(const Vec2i& comparable) const {
	return x != comparable.x || y != comparable.y;
}