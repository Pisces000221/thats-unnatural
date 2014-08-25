#include "LevelPlayScene.h"
#include "Bricks.h"
using namespace cocos2d;

level_reader::level LevelPlay__0_ltl;
const char *LevelPlay::readyMsg[6] = { "10", "9", "8", "6", "3", "DROP" };

bool LevelPlay::init(PhysicsWorld *world)
{
    if (!FreePhysics::init(world)) return false;
    this->removeChildByTag(FreePhysics::TAG_DASHBOARD);
    _level = LevelPlay__0_ltl;

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

    // Load enabled brick types
    _enabledBrickTypes = 0x0;
    for (int i = 0; i < bricks::BRICK_TYPE_COUNT; i++) {
        if (_level.brick_type_enabled[i]) _enabledBrickTypes |= (1 << i);
    }

    // Delay some time before starting
    // There is a delay of 0.4s before transition was finished
    // So we use 3s(time to delay) + 0.4s
    _timeSinceLastBrickGen = -3.4;
    float deltay = 30;
    for (int i = 0; i < 6; i++) {
        auto lbl_ready = LABEL(readyMsg[i], 64, "Bold");
        lbl_ready->setPosition(Vec2(_contentSize.width * 0.5, _contentSize.height * 0.5 - deltay));
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

