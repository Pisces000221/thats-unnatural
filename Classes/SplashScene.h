#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"

class Splash : public cocos2d::Layer
{
public:
    virtual bool init(cocos2d::PhysicsWorld *world);
    void close(cocos2d::Ref *sender);
    PHY_CREATE_FUNC(Splash);
    PHY_SCENE_FUNC(Splash);
};

#endif
