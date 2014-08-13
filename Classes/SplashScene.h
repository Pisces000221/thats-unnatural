#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"

class Splash : public cocos2d::Layer
{
public:
    virtual bool init();
    void close(cocos2d::Ref *sender);
    CREATE_FUNC(Splash);
    SCENE_FUNC(Splash);
};

#endif
