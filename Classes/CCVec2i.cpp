#include "CCVec2i.h"

using namespace cocos2d;

CCVec2i::CCVec2i() : x(0), y(0) {}
CCVec2i::CCVec2i(int x, int y) : x(x), y(y) {}
CCVec2i::CCVec2i(const cocos2d::CCPoint& vec2f) : x((int)vec2f.x), y((int)vec2f.y) {}
CCVec2i::CCVec2i(const cocos2d::CCSize& size) : x((int)size.width), y((int)size.height) {}
CCVec2i::CCVec2i(int xy) : x(xy), y(xy) {}
CCVec2i::CCVec2i(const CCVec2i& copyObj) : x(copyObj.x), y(copyObj.y) {}

bool CCVec2i::operator==(const CCVec2i& comparable) const {
	return x == comparable.x && y == comparable.y;
}
