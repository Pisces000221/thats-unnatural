#include "LevelPlayScene.h"
#include "Bricks.h"
using namespace cocos2d;
using level_reader::level_objective;

level_reader::level LevelPlay__0_ltl;
const char *LevelPlay::readyMsg[6] = { "10", "9", "8", "6", "3", "DROP" };

#define ADD_TO_V_ALIGN(__child) do { \
    __child->setPosition(Vec2(size.width, _lastYPos)); \
    _lastYPos += __child->getContentSize().height * __child->getScale(); \
    this->addChild(__child, 10000); \
} while (0)

bool LevelPlay::init(PhysicsWorld *world)
{
    if (!FreePhysics::init(world)) return false;
    this->removeChildByTag(FreePhysics::TAG_DASHBOARD);
    _level = LevelPlay__0_ltl;
    auto size = _contentSize;
    _lastYPos = 0;

    // My own title
    char s[12]; sprintf(s, "LEVEL %02d", _level.id);
    auto lbl_title = LABEL(s, 48, "Bold");
    lbl_title->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    lbl_title->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    this->addChild(lbl_title, 10000);   // On the top (Mountain top!!)
    lbl_title->runAction(Sequence::create(
        DelayTime::create(5),
        FadeOut::create(1),
        RemoveSelf::create()
    , nullptr));

    // The timer in the bottom-right corner
    if (_level.tot_time > 0) {
        _timer = Timepiece::create(Color4B(128, 128, 128, 128), 75);
        _timer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        ADD_TO_V_ALIGN(_timer);
    } else _timer = nullptr;

    this->getScheduler()->schedule(
        CC_CALLBACK_1(LevelPlay::time_tick, this),
        this, 0, false, "LEVEL_TOTTIME_TICK");

    // Load enabled brick types
    _enabledBrickTypes = 0x0;
    for (int i = 0; i < bricks::BRICK_TYPE_COUNT; i++) {
        if (_level.brick_type_enabled[i]) _enabledBrickTypes |= (1 << i);
    }

    // Minimum height mode
    if (_level.objective == level_objective::MIN_HEIGHT) {
        _sensorLine->setPositionY(size.height * _level.min_height);
        _endurtimer = Timepiece::create(Color4B(128, 128, 255, 128), 75);
        _endurtimer->setOpacity(0);
        _endurtimer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        _endurtime = 0;
        ADD_TO_V_ALIGN(_endurtimer);
        CCLOG("endurance: %f", _level.endurance_time);
    } else {
        _sensorLine->removeFromParent();
        _sensorLine = nullptr;
        _useSensor = false;
    }
    // Maximum hit bricks / hit count mode
    if (_level.objective == level_objective::MAX_HITBRICKS
            || _level.objective == level_objective::MAX_HITCOUNT) {
        // The counter
        _lbl_count = LABEL("", 48);
        _count = 0; refreshLabel();
        _lbl_count->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        ADD_TO_V_ALIGN(_lbl_count);
    } else {
        _useMoistening = false;
    }

    // Delay some time before starting
    // There is a delay of 0.4s before transition was finished
    // So we use 3s(time to delay) + 0.4s
    _tottime = _timeSinceLastBrickGen = -3.4;
    float deltay = 30;
    for (int i = 0; i < 6; i++) {
        auto lbl_ready = LABEL(readyMsg[i], 64, "Bold");
        lbl_ready->setPosition(Vec2(size.width * 0.5, size.height * 0.5 - deltay));
        lbl_ready->setOpacity(0);
        lbl_ready->runAction(Sequence::create(
            DelayTime::create(0.4 + i * 0.5),
            Spawn::create(FadeIn::create(0.15), MoveBy::create(0.2, Vec2(0, deltay)), nullptr),
            DelayTime::create(0.35),
            Spawn::create(FadeOut::create(0.15), MoveBy::create(0.2, Vec2(0, deltay)), nullptr),
            RemoveSelf::create(),
        nullptr));
        this->addChild(lbl_ready);
    }

    return true;
}

void LevelPlay::refreshLabel()
{
    char s[20]; sprintf(s, "%d / %d", _count, _level.max_hit);
    _lbl_count->setString(s);
}

void LevelPlay::time_tick(float dt)
{
    if (_lineReached) {
        _endurtime += dt;
        _endurtimer->setTime(1 - _endurtime / _level.endurance_time);
    } else if (_level.tot_time > 0) {
        _tottime += dt;
        if (_tottime > 0)
            _timer->setTime(1 - _tottime / _level.tot_time);
    }
}

void LevelPlay::trayHit(PhysicsBody *a, PhysicsBody *b)
{
    FreePhysics::trayHit(a, b);
    if (_level.objective == level_objective::MAX_HITCOUNT
            || (_level.objective == level_objective::MAX_HITBRICKS &&
            _newBrickMoistened)) {
        _count++;
        refreshLabel();
    }
}

void LevelPlay::lineAttach()
{
    FreePhysics::lineAttach();
    _endurtimer->runAction(FadeTo::create(0.1, 128));
}

void LevelPlay::lineDetach()
{
    FreePhysics::lineDetach();
    _endurtimer->runAction(FadeOut::create(0.1));
    _endurtime = 0;
}

