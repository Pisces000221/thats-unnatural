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
    static LevelList *getInstance();    // You can call addLevel() on this

    // Set appending to true to drop from the top
    void addLevel(bool appending = true); 

    static const int GROUP_COLOUR_COUNT = 6;
    static const cocos2d::Color3B groupColours[GROUP_COLOUR_COUNT];

protected:
    cocos2d::Layer *_frontLayer;    // The physics bodies are placed here

    bool onTouchBegan(Touch *, Event *);
    void onTouchMoved(Touch *, Event *);
    void onTouchEnded(Touch *, Event *);
    void resetSelected();           // Reset the colour of the selected brick
    void enterLevel(int);

    int _levelsDisplayed;
    float _maxHeight;   // The biggest brick's height, not used after init()
    cocos2d::Node *_lastLevelBrick; // Its colour will be brought back
    cocos2d::Node *_selectedNode;   // Selected brick (tapped but not released)
    int _selectedLevel;             // Selected level number (starts from 1)
    cocos2d::Color3B _lastColour;   // The initial colour of the selected brick
    bool _dragStarted;
    float _dragStartPosY;
    
    static const int TAG_LEVEL_0 = 0xC0C052D;
};

#endif

