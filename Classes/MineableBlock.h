#ifndef __MINEABLE_BLOCK_H__
#define __MINEABLE_BLOCK_H__

#include "GridObject.h"

class MineableBlock : public GridObject {
public:
	CREATE_FUNC(MineableBlock);

	MineableBlock();
	
	bool init() override;
	GridObject::OverlapResult onPlayerOverlap(GridObject::OverlapContext& context) override;
protected:
	int m_maxStrength;
	int m_estimatedStrength;
};

#endif //!__MINEABLE_BLOCK_H__