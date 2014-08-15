#include "AppMacros.h"

#include "StartupScene.h"
using namespace cocos2d;

bool Startup::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    Size size = Director::getInstance()->getVisibleSize();

    // Create the edge box
    auto edge_body = PhysicsBody::createEdgeBox(size, PHYSICSBODY_MATERIAL_DEFAULT, 3);
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
        auto gangti = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
        gangti->setMass(1);
        gangti->applyForce(Vec2(150.0 * RAND_0_1 - 80.0, 150.0 * RAND_0_1 - 80.0));
        sprite->setPhysicsBody(gangti);    // Well, 'body' in Chinese...
        this->addChild(sprite);
    }

    // The title
    //PhysicsMaterial heavy_material(1e6, 0, 0);
    auto lbl_thats = LABEL("That\'s", 40);
    lbl_thats->setPosition(Vec2(size.width * 0.5, size.height * 0.6));
    auto body_thats = PhysicsBody::createBox(lbl_thats->getContentSize());
    body_thats->setMass(3);
    lbl_thats->setPhysicsBody(body_thats);
    this->addChild(lbl_thats);
    auto lbl_unn = LABEL("UNNATURAL!", 48, "Bold");
    lbl_unn->setPosition(Vec2(size.width * 0.5, size.height * 0.45));
    auto body_unn = PhysicsBody::createBox(lbl_unn->getContentSize());
    body_unn->setMass(4);
    lbl_unn->setPhysicsBody(body_unn);
    this->addChild(lbl_unn);

    return true;
}

