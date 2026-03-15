#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "ObjectEventDelegate.h"

class GameObject;

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
private:
	cocos2d::Vector<GameObject*> m_allObjects;
};

#endif //!__PLAY_LAYER_H__