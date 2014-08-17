#ifndef __LEVELLIST_SCENE_H__
#define __LEVELLIST_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"

class LevelList : public cocos2d::LayerColor
{
public:
    LevelList();
    virtual bool init(cocos2d::PhysicsWorld *world);
    PHY_CREATE_FUNC(LevelList);
    PHY_SCENE_FUNC(LevelList);

    static const int GROUP_COLOUR_COUNT = 6;
    static const cocos2d::Color3B groupColours[GROUP_COLOUR_COUNT];

protected:
    cocos2d::Layer *_frontLayer;

    bool onTouchBegan(Touch *, Event *);
    void onTouchMoved(Touch *, Event *);
    void onTouchEnded(Touch *, Event *);
    void resetSelected();

    cocos2d::Node *_selectedNode;
    cocos2d::Color3B _lastColour;
    bool _dragStarted;
    float _dragStartPosY;
};

#endif

