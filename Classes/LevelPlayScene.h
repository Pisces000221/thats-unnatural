#ifndef __LEVEL_PLAY_SCENE_H__
#define __LEVEL_PLAY_SCENE_H__

#include "cocos2d.h"
#include "data/level_reader.h"
#include "widgets/Timepiece.h"
#include "RetriableProtocol.h"
#include "FreePhysicsScene.h"

extern level_reader::level LevelPlay__0_ltl;

class LevelPlay : public FreePhysics, public RetriableProtocol
{
public:
    // Call this before calling createScene()
    static void setLevelToLoad(level_reader::level l)
    { LevelPlay__0_ltl = l; }
    virtual bool init(cocos2d::PhysicsWorld *world) override;
    PHY_CREATE_FUNC(LevelPlay);
    PHY_SCENE_FUNC(LevelPlay);
    virtual void retry() override;

    static const char *readyMsg[6];

protected:
    level_reader::level _level;
    void time_tick(float);
    void refreshLabel();
    void gameOver(std::string);
    virtual void trayHit(PhysicsBody *, PhysicsBody *) override;
    virtual void lineAttach() override;
    virtual void lineDetach() override;

    float _lastYPos;
    cocos2d::Label *_lbl_count;
    int _count; // For recording the hits
    Timepiece *_timer, *_endurtimer;
    float _tottime, _endurtime;
    bool _gameEnded;
};

#endif

