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

protected:
    bool onTouchBegan(Touch *, Event *);
    void onTouchMoved(Touch *, Event *);
    void onTouchEnded(Touch *, Event *);

    // Stores touches that are not released
    std::unordered_map<int, Node *> _nails;
};

#endif

