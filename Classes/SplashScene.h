#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"

class Splash : public cocos2d::LayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(Splash);
    SCENE_FUNC(Splash);
};

#endif
