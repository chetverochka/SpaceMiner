#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "ObjectEventDelegate.h"
#include "ChunkMap.h"

class GameObject;
class PlayerObject;

class PlayLayer : public cocos2d::CCLayer, public ObjectEventDelegate {
public:
	CREATE_FUNC(PlayLayer);

	PlayLayer();
	~PlayLayer();

	// CC overrides
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaTime) override;
	void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

	void onObjectCellMoved(GameObject* target, int destinationX, int destinationY) override;

	void addObject(GameObject* object);
	void removeObject(GameObject* object);
protected:
	virtual void drawChunks();
	virtual void ccKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
	virtual void ccKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
private:
	cocos2d::Vec2 m_gridStep;
	ChunkMap* m_chunkMap;
	cocos2d::Vector<GameObject*> m_allObjects;
	PlayerObject* m_player;
	cocos2d::DrawNode* m_drawCanvas;
	cocos2d::CustomCommand m_drawChunksCommand;
};

#endif //!__PLAY_LAYER_H__