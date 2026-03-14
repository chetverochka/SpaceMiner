#ifndef __PLAY_LAYER_H__
#define __PLAY_LAYER_H__

#include "cocos2d.h"
#include "ObjectEventDelegate.h"

class PlayLayer : public cocos2d::CCLayer, public ObjectEventDelegate {
public:
	CREATE_FUNC(PlayLayer);

	PlayLayer();

	bool init() override;
};

#endif //!__PLAY_LAYER_H__