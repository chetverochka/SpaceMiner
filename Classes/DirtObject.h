#ifndef __DIRT_OBJECT_H__
#define __DIRT_OBJECT_H__

#include "GameObject.h"

class DirtObject : public GameObject {
public:
	CREATE_FUNC(DirtObject);

	bool init() override;
};

#endif //!__DIRT_OBJECT_H__