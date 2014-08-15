#ifndef __FREE_PHY_SCENE_H__
#define __FREE_PHY_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"

class FreePhysics : public cocos2d::LayerColor
{
public:
    virtual bool init(cocos2d::PhysicsWorld *world);
    GO_BACK_FUNC;
    PHY_CREATE_FUNC(FreePhysics);
    PHY_SCENE_FUNC(FreePhysics);
};

#endif

