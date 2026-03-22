#ifndef __MINEABLE_OBJECT_H__
#define __MINEABLE_OBJECT_H__

#include "GridObject.h"

class MineableObject : public GridObject {
public:
	enum MineableType {
		DEFAULT,
		DIAMOND,
		GOLD,
		COPPER,
		OBSIDIAN
	};

	CREATE_FUNC(MineableObject);

	MineableObject();
	
	bool init() override;

	void setMaxStrength(const int maxStrength);

	int getMaxStrength() const;
	int getEstimatedStrength() const;

	void mine(const int strength);
	void resetMine();

	bool isBroken();
protected:
	virtual void onMiningInteraction(const int currentStrength, const int maxStrength);

	int m_maxStrength;
	int m_estimatedStrength;

};

#endif //!__MINEABLE_OBJECT_H__