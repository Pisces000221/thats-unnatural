#ifndef __STARTUP_SCENE_H__
#define __STARTUP_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"

class Startup : public cocos2d::LayerColor
{
public:
    virtual bool init(cocos2d::PhysicsWorld *world);
    PHY_CREATE_FUNC(Startup);
    PHY_SCENE_FUNC(Startup);
};

#endif

