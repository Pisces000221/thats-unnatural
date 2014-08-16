#include "AppMacros.h"
#include "Bricks.h"
#include "FreePhysicsScene.h"
#include "LevelListScene.h"

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
    world->setDebugDrawMask(0);
    // The unnatural gravity
    world->setGravity(Vec2(0, 0));
    // Create fifteen random objects
    for (int i = 0; i < 15; i++) {
        auto obj = bricks::new_random(24);
        obj->setPosition(Vec2(size.width * RAND_0_1, size.height * RAND_0_1));
        obj->getPhysicsBody()
            ->applyForce(Vec2(15.0 * RAND_0_1 - 8.0, 15.0 * RAND_0_1 - 8.0) * 1000);
        this->addChild(obj);
    }

    // Tap the bigger brick 10 times to toggle debug draw visibility
    auto item_toggledbg = MenuItem::create([world](Ref *sender) {
        int count = ((Node *)sender)->getTag();
        if (++count >= 10) {
            world->setDebugDrawMask(1 - world->getDebugDrawMask());
            count = 0;
        }
        ((Node *)sender)->setTag(count);
    });
    auto brick_toggledbg = bricks::new_random(40);
    bricks::set_brick_colour(brick_toggledbg, Color3B::YELLOW);
    item_toggledbg->addChild(brick_toggledbg);
    item_toggledbg->setPosition(Vec2(100, 400));
    item_toggledbg->setPhysicsBody(brick_toggledbg->getPhysicsBody());
    brick_toggledbg->setAnchorPoint(Vec2::ZERO);
    item_toggledbg->setContentSize(brick_toggledbg->getContentSize());

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
    auto item_levels = MenuItemLabel::create(
        LABEL("Levels", 40, "Bold"),
        [this](Ref *sender) { GO_TO_SCENE(LevelList); });
    item_levels->setPosition(Vec2(size.width * 0.382, size.height * 0.3));
    auto item_freemode = MenuItemLabel::create(
        LABEL("Free mode", 40, "Bold"),
        [this](Ref *sender) { GO_TO_SCENE(FreePhysics); });
    item_freemode->setPosition(Vec2(size.width * 0.382, size.height * 0.2));
    auto menu = Menu::create(item_toggledbg, item_levels, item_freemode, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

