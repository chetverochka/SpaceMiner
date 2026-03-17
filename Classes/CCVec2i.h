#ifndef __CC_VEC2I_H__
#define __CC_VEC2I_H__

#include "math/Vec2.h"
#include "deprecated/CCDeprecated.h"

namespace cocos2d {
	class CCVec2i {
	public:
		int x;
		int y;

		CCVec2i();
		CCVec2i(int x, int y);
		CCVec2i(const cocos2d::CCPoint& vec2f);
		CCVec2i(const cocos2d::CCSize& size);
		CCVec2i(int xy);
		CCVec2i(const CCVec2i& copyObj);

		bool operator==(const CCVec2i& comparable) const;
		bool operator!=(const CCVec2i& comparable) const;
	};
}

#endif //!__CC_VEC2I_H__