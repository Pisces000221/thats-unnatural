#include "AppMacros.h"

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

    return true;
}

