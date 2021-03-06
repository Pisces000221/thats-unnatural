#ifndef __FREE_PHY_SCENE_H__
#define __FREE_PHY_SCENE_H__

#include <unordered_map>
#include <unordered_set>
#include "cocos2d.h"
#include "AppMacros.h"
#include "Bricks.h"

class FreePhysics : public cocos2d::LayerColor
{
public:
    FreePhysics() : _timeSinceLastBrickGen(0), _timeBtwnBrickGen(0),
        _lineTouchCount(0), _minID(MIN_BRICK_ID), _maxID(MIN_BRICK_ID - 1),
        _useSensor(true), _useMoistening(true), _lineReached(false) {}
    virtual bool init(cocos2d::PhysicsWorld *world);
    PHY_CREATE_FUNC(FreePhysics);
    PHY_SCENE_FUNC(FreePhysics);

protected:
    bool onTouchBegan(Touch *, Event *);
    void onTouchMoved(Touch *, Event *);
    void onTouchEnded(Touch *, Event *);
    bool onContactBegin(PhysicsContact &);
    void onContactSeperate(PhysicsContact &);
    virtual void lineAttach();
    virtual void lineAboutDetach();
    virtual void lineDetach() {} // = 0;
    virtual void trayHit(PhysicsBody *, PhysicsBody *);
    void generateBrick();
    void autoCullBricks(float);
    float _timeSinceLastBrickGen, _timeBtwnBrickGen;

    // Stores touches that are not released
    std::unordered_map<int, Node *> _nails;
    // Stores bricks that have hit the tray
    std::unordered_map<int, clock_t> _moistenTime;

    bricks::etype _enabledBrickTypes;
    cocos2d::Node *_sensorLine;
    int _lineTouchCount;    // How many bricks have now reached the line
    int _minID, _maxID;     // Smallest and biggest IDs of bricks
    static const float LINE_DETACH_MAX_TIME;
    static const int SENSOR_ID = 0;
    static const int TRAY_ID = 1;
    static const int MIN_BRICK_ID = 2;
    static const int BRICKS_GROUP = 0x1337;     // Keep this positive
    static const int TAG_DASHBOARD = 0X0FE21F;  // What??? Firefox?

    // Settings by LevelPlay
    bool _useSensor;
    bool _useMoistening;
    // Info used by LevelPlay
    bool _newBrickMoistened;
    bool _validHitCount;
    bool _lineReached;
};

#endif

