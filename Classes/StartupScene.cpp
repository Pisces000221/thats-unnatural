#include "AppMacros.h"
#include "FreePhysicsScene.h"

#include "StartupScene.h"
using namespace cocos2d;

bool Startup::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    Size size = Director::getInstance()->getVisibleSize();

    // Materials
    PhysicsMaterial light_material(1, 0, 0);
    PhysicsMaterial normal_material(2, 0, 0);
    PhysicsMaterial heavy_material(3, 0, 0);
    // Create the edge box
    auto edge_body = PhysicsBody::createEdgeBox(size, PHYSICSBODY_MATERIAL_DEFAULT, 1);
    auto edge_node = Node::create();
    edge_node->setPosition(Vec2(size.width / 2, size.height / 2));
    edge_node->setPhysicsBody(edge_body);
    this->addChild(edge_node);
    // The unnatural gravity
    world->setGravity(Vec2(0, 0));
    // Create fifteen random objects
    for (int i = 0; i < 15; i++) {
        auto sprite = Sprite::create("circle.png");
        sprite->setPosition(Vec2(size.width * RAND_0_1, size.height * RAND_0_1));
        auto gangti = PhysicsBody::createCircle(sprite->getContentSize().width / 2, normal_material);
        gangti->applyForce(Vec2(15.0 * RAND_0_1 - 8.0, 15.0 * RAND_0_1 - 8.0) * 1000);
        sprite->setPhysicsBody(gangti);     // Well, 'body' in Chinese...
        this->addChild(sprite);
    }

    // The title
    auto lbl_thats = LABEL("That\'s", 40);
    lbl_thats->setPosition(Vec2(size.width * 0.5, size.height * 0.6));
    auto body_thats = PhysicsBody::createBox(lbl_thats->getContentSize(), heavy_material);
    lbl_thats->setPhysicsBody(body_thats);
    this->addChild(lbl_thats);
    auto lbl_unn = LABEL("UNNATURAL!", 48, "Bold");
    lbl_unn->setPosition(Vec2(size.width * 0.5, size.height * 0.45));
    auto body_unn = PhysicsBody::createBox(lbl_unn->getContentSize(), heavy_material);
    lbl_unn->setPhysicsBody(body_unn);
    this->addChild(lbl_unn);

    // The menus
    // Free mode
    auto item_freemode = MenuItemLabel::create(
        LABEL("Free mode", 40, "Bold"),
        [this](Object *sender) { GO_TO_SCENE(FreePhysics); });
    item_freemode->setPosition(Vec2(size.width * 0.25, size.height * 0.3));
    auto menu = Menu::create(item_freemode, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

