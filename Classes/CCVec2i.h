#ifndef __CC_VEC2I_H__
#define __CC_VEC2I_H__

#include "math/Vec2.h"
#include "deprecated/CCDeprecated.h"

namespace cocos2d {
	class Vec2i {
	public:
		int x;
		int y;

		Vec2i();
		Vec2i(int x, int y);
		Vec2i(const cocos2d::CCPoint& vec2f);
		Vec2i(const cocos2d::CCSize& size);
		Vec2i(int xy);
		Vec2i(const Vec2i& copyObj);

		bool operator==(const Vec2i& comparable) const;
		bool operator!=(const Vec2i& comparable) const;
	};
}

#endif //!__CC_VEC2I_H__