#ifndef __MINEABLE_OBJECT_H__
#define __MINEABLE_OBJECT_H__

#include "GridObject.h"

class MineableObject : public GridObject {
public:
	static const float BASE_OBJECT_SCALE;

	enum MineableType {
		DEFAULT,
		DIAMOND,
		GOLD,
		COPPER,
		OBSIDIAN
	};

	struct CreateData {
		int maxStrength;

	};

	CREATE_FUNC(MineableObject);

	static MineableObject* createWithType(MineableType type);
	static MineableObject* create(CreateData createData);
	MineableObject();
	
	bool initWithValues(CreateData createData);
	bool init() override;

	//void setMaxStrength(const int maxStrength);

	int getMaxStrength() const;
	int getEstimatedStrength() const;

	void mine(const int strength);
	void resetMine();

	bool isBroken();

	void setMineableType(MineableObject::MineableType type);
	const MineableType& getMineableType() const;
protected:
	virtual void onMiningInteraction(const int currentStrength, const int maxStrength);

	int m_maxStrength;
	int m_estimatedStrength;
	MineableType m_mineableType;

};

#endif //!__MINEABLE_OBJECT_H__