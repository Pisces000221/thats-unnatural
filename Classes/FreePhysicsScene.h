#ifndef __FREE_PHY_SCENE_H__
#define __FREE_PHY_SCENE_H__

#include "cocos2d.h"
#include "AppMacros.h"
#include "Bricks.h"

class FreePhysics : public cocos2d::LayerColor
{
public:
    FreePhysics() : _lineTouchCount(0), _minID(2), _maxID(1) {}
    virtual bool init(cocos2d::PhysicsWorld *world);
    PHY_CREATE_FUNC(FreePhysics);
    PHY_SCENE_FUNC(FreePhysics);

protected:
    bool onTouchBegan(Touch *, Event *);
    void onTouchMoved(Touch *, Event *);
    void onTouchEnded(Touch *, Event *);
    bool onContactBegin(PhysicsContact &);
    void onContactSeperate(PhysicsContact &);
    void lineAttach();
    void lineDetach();
    void generateBrick(float);
    void autoCullBricks(float);

    // Stores touches that are not released
    std::unordered_map<int, Node *> _nails;

    bricks::etype _enabledBrickTypes;
    cocos2d::Node *_sensorLine;
    int _lineTouchCount;    // How many bricks have now reached the line
    int _minID, _maxID;     // Smallest and biggest IDs of bricks
    static const float LINE_DETACH_MAX_TIME;
    static const int SENSOR_ID = 0;
    static const int TRAY_ID = 1;
    static const int MIN_BRICK_ID = 2;
    static const int BRICKS_GROUP = 0x1337;     // Keep this positive
};

#endif

