#include "AppMacros.h"
#include "Bricks.h"
#include "widgets/GravityPicker.h"

#include "FreePhysicsScene.h"
using namespace cocos2d;

#define TAG_DRAGGABLE 0x80
#define BRICK_INIT_Y_OFFSET 30
#define CULLING_BOUND -100

bool FreePhysics::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    Size size = Director::getInstance()->getVisibleSize();
    CREATE_GO_BACK_MENUITEM;

    // Create physics world
    // Let's get a tray
    auto tray_body = PhysicsBody::createEdgeBox(Size(200, 20));
    auto tray_node = Sprite::create("images/white_pixel.png");
    tray_node->setPosition(Vec2(size.width * 0.5, 20));
    tray_node->setScale(200, 20);
    tray_node->setColor(Color3B::YELLOW);
    tray_node->setPhysicsBody(tray_body);
    this->addChild(tray_node);
    // Turn on timer
    this->getScheduler()->schedule([this, size](float dt) {
        // Generate a brick with a random shape
        auto obj = bricks::new_random(24);
        obj->setPosition(Vec2(size.width * RAND_0_1, size.height + BRICK_INIT_Y_OFFSET));
        obj->getPhysicsBody()->setTag(TAG_DRAGGABLE);
        this->addChild(obj);
    }, this, 0.2, false, "FREEPHYSICS_GEN");

    CCLOG("Default gravity: (%.5f, %.5f)", world->getGravity().x, world->getGravity().y);
    // A gravity picker
    // TODO: Put this one in a dashboard that can be hidden
    auto picker = GravityPicker::create();
    picker->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    picker->setNormalizedPosition(Vec2(0.95, 0.2));
    picker->valueChangedCallback = [world](Vec2 gravity) {
        world->setGravity(gravity);
    };
    this->addChild(picker);

    // Enable touching
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FreePhysics::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(FreePhysics::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(FreePhysics::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Auto-culling for bodies
    this->getScheduler()->schedule([this, world](float dt) {
        const Vector<PhysicsBody *> bodies = world->getAllBodies();
        for (auto body: bodies) {
            if (body->getPosition().y < CULLING_BOUND) world->removeBody(body);
        }
    }, this, 0.1, false, "BODIES_AUTO_CULLING");

    return true;
}

// Enable dragging
// Copied from cpp-tests/PhysicsTest
bool FreePhysics::onTouchBegan(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    auto arr = this->getScene()->getPhysicsWorld()->getShapes(location);

    PhysicsBody *body = nullptr;
    for (auto &obj: arr) {
        if ((obj->getBody()->getTag() & TAG_DRAGGABLE)) {
            body = obj->getBody();
            break;
        }
    }

    if (body != nullptr) {
        // Get a nail to fix the selected object, stop them from moving and colliding
        Node *nail = Node::create();
        nail->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        nail->setPhysicsBody(PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY));
        nail->getPhysicsBody()->setDynamic(false);
        nail->setPosition(location);
        this->addChild(nail);
        PhysicsJointPin *joint = PhysicsJointPin::construct(nail->getPhysicsBody(), body, location);
        joint->setMaxForce(5000.0f * body->getMass());
        this->getScene()->getPhysicsWorld()->addJoint(joint);
        _nails.insert(std::make_pair(touch->getID(), nail));

        return true;
    } else return false;
}

void FreePhysics::onTouchMoved(Touch *touch, Event *event)
{
    auto it = _nails.find(touch->getID());
    if (it != _nails.end()) it->second->setPosition(touch->getLocation());
}

void FreePhysics::onTouchEnded(Touch *touch, Event *event)
{
    auto it = _nails.find(touch->getID());
    if (it != _nails.end()) {
        this->removeChild(it->second);
        _nails.erase(it);
    }
}

