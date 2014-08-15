#include "AppMacros.h"
#include "Bricks.h"

#include "FreePhysicsScene.h"
using namespace cocos2d;

bool FreePhysics::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    Size size = Director::getInstance()->getVisibleSize();

    CREATE_GO_BACK_MENUITEM(FreePhysics, item_back);
    auto menu = Menu::create(item_back, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    // Create physics world
    this->getScheduler()->schedule([this, size](float dt) {
        // Generate a random object
        auto obj = bricks::new_random(24);
        obj->setPosition(Vec2(size.width * RAND_0_1, size.height + 12));
        this->addChild(obj);
    }, this, 0.2, false, "FREEPHYSICS_GEN");

    return true;
}

