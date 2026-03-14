#ifndef __PLAY_SCENE_H__
#define __PLAY_SCENE_H__

#include "cocos2d.h"

class PlayScene : public cocos2d::CCScene {
public:
	CREATE_FUNC(PlayScene);

	PlayScene();

	bool init() override;

};

#endif //!__PLAY_SCENE_H__