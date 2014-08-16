#ifndef __LEVELLIST_SCENE_H__
#define __LEVELLIST_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"

class LevelList : public cocos2d::LayerColor
{
public:
    virtual bool init(cocos2d::PhysicsWorld *world);
    PHY_CREATE_FUNC(LevelList);
    PHY_SCENE_FUNC(LevelList);
};

#endif

