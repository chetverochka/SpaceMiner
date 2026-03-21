#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "CCVec2i.h"

class GameObject;
class PlayerObject;

class PlayLayer : public cocos2d::CCLayer {
public:
	static const int PLAYER_MOVE_ACTION_TAG;

	CREATE_FUNC(PlayLayer);

	PlayLayer();
	~PlayLayer();

	// CC overrides
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaTime) override;
	void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

	cocos2d::Vec2 getGridStep();

protected:
	virtual void debugDraw();
	virtual void ccKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
	virtual void ccKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
private:
	cocos2d::CustomCommand m_drawCommand;
	cocos2d::DrawNode* m_drawCanvas;
	cocos2d::Sprite* m_playerSprite;

	cocos2d::Vec2i m_playerCell;
};




#endif //!__PLAY_LAYER_H__