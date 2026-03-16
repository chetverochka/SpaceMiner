#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "cocos2d.h"
#include "ChunkMap.h"

class ObjectEventDelegate;

class GameObject : public cocos2d::CCSprite, public ChunkObjectDelegate {
public:
	static ObjectEventDelegate ms_safeEventDelegate;
	CREATE_FUNC(GameObject);

	GameObject();

	void update(float deltaTime) override;
	void onEnter() override;
	void onExit() override;

	void setCellX(int x) override;
	void setCellY(int y) override;
	void setCell(int x, int y) override;

	int getCellX() const override;
	int getCellY() const override;
	void getCell(int* x, int* y) const override;
private:
	void updatePosition();

	ObjectEventDelegate* m_eventDelegate;
	int m_cellX, m_cellY;
};

#endif //!__GAME_OBJECT_H__