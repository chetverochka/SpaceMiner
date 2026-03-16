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

	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaTime) override;

	void addObject(GameObject* object);
	void removeObject(GameObject* object);
protected:
	virtual void ccKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
	virtual void ccKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
private:
	ChunkMap m_chunkMap;
	cocos2d::Vector<GameObject*> m_allObjects;
	PlayerObject* m_player;
};

#endif //!__PLAY_LAYER_H__