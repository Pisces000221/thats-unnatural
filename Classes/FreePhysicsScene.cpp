#include "algo/indirect_touch.h"
#include "AppMacros.h"
#include "Bricks.h"
#include "widgets/Dashboard.h"

#include "FreePhysicsScene.h"
using namespace cocos2d;

#define DRAGGABLE_MIN_ID 2
#define DATA_IGNORED_CONTACT 0x5f3759df // Uh...
#define BRICK_INIT_Y_OFFSET 30  // A newly generated brick will be height+30 points high
#define CULLING_BOUND -100      // If a brick's Y position is below this, we say bye bye to it

bool FreePhysics::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    Size size = Director::getInstance()->getVisibleSize();
    CREATE_GO_BACK_MENUITEM;

    // Create physics world
    // Let's get a tray
    auto tray_body = PhysicsBody::createEdgeBox(Size(200, 20));
    tray_body->setTag(TRAY_ID);
    tray_body->setContactTestBitmask(0xFFFFFFFF);
    auto tray_node = Sprite::create("images/white_pixel.png");
    tray_node->setPosition(Vec2(size.width * 0.5, 20));
    tray_node->setScale(200, 20);
    tray_node->setColor(Color3B::YELLOW);
    tray_node->setPhysicsBody(tray_body);
    this->addChild(tray_node);

    CCLOG("Default gravity: (%.5f, %.5f)", world->getGravity().x, world->getGravity().y);

    // The dashboard
    auto dashboard = Dashboard::create();
    dashboard->setPosition(Vec2(size.width, 0));
    this->addChild(dashboard);
    dashboard->addLabel("GRAVITY");
    dashboard->addGravityPicker([world](Vec2 gravity) {
        world->setGravity(gravity);
    });
    dashboard->addLabel("BRICKS");
    const std::string brick_names[bricks::BRICK_TYPE_COUNT]
        = { "Balls", "Boxes", "Triangles" };
    _enabledBrickTypes = bricks::FLAG_ALL_ENABLED;
    for (int i = 0; i < bricks::BRICK_TYPE_COUNT; i++) {
        int _i = i;
        dashboard->addTickButton(brick_names[i], [this, _i](bool b) {
            // http://stackoverflow.com/questions/47981
            // http://stackoverflow.com/questions/6916974
            if (b) _enabledBrickTypes |= 1 << _i;
            else _enabledBrickTypes &= ~(1 << _i);
        }, _enabledBrickTypes & 1 << _i);
    }

    // A sensor line
    auto line = bricks::new_sensorline(
        Vec2(0, size.height * 0.6), Vec2(size.width, size.height * 0.6));
    line->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
    line->getPhysicsBody()->setCollisionBitmask(0x0);
    line->getPhysicsBody()->setTag(SENSOR_ID);
    this->addChild(line);

    indirect_touch::init();
    indirect_touch::start = TRAY_ID;
    indirect_touch::finish = SENSOR_ID;
    // Contact listener
    // http://www.cnblogs.com/JiaoQing/p/3906780.html
    // http://childhood.logdown.com/posts/192612/chipmunk-collision-in-detailed-cocos2dx-filter
    // https://github.com/chukong/cocos-docs/blob/master/manual/framework/native/wiki/physics/zh.md
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this, line](PhysicsContact &contact) {
        CCLOG("contact begin");
        indirect_touch::add_arc(
            contact.getShapeA()->getBody()->getTag(),
            contact.getShapeB()->getBody()->getTag());
        bricks::set_brick_colour(line,
            indirect_touch::calculate() ? Color3B::GREEN : Color3B::RED);
        return true;
    };
    contactListener->onContactSeperate = [this, line](PhysicsContact &contact) {
        indirect_touch::remove_arc(
            contact.getShapeA()->getBody()->getTag(),
            contact.getShapeB()->getBody()->getTag());
        bricks::set_brick_colour(line,
            indirect_touch::calculate() ? Color3B::GREEN : Color3B::RED);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Turn on timer
    this->getScheduler()->schedule([this, size](float dt) {
        // Generate a brick with a random shape
        auto obj = bricks::new_random(24, _enabledBrickTypes);
        obj->setPosition(Vec2(size.width * RAND_0_1, size.height + BRICK_INIT_Y_OFFSET));
        int tag;
        if (rand () % 2 && _minID > MIN_BRICK_ID + 1) tag = --_minID; else tag = ++_maxID;
        obj->getPhysicsBody()->setTag(tag);
        //CCLOG("New ID: %d", tag);
        obj->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
        obj->getPhysicsBody()->setGroup(BRICKS_GROUP);
        this->addChild(obj);
    }, this, 0.2, false, "FREEPHYSICS_GEN");

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
            if (body->getPosition().y < CULLING_BOUND) {
                indirect_touch::remove_all_arcs(body->getTag());
                //CCLOG("Remove ID: %d", body->getTag());
                if (body->getTag() == _minID)
                    while (world->getBody(++_minID) == nullptr) {}
                else if (body->getTag() == _maxID)
                    while (world->getBody(--_maxID) == nullptr) {}
                world->removeBody(body);
            }
        }
    }, this, 0.1, false, "BODIES_AUTO_CULLING");

    return true;
}

// Enable dragging
// Copied from cpp-tests/PhysicsTest
bool FreePhysics::onTouchBegan(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    // Find a draggable object at the current position
    auto arr = this->getScene()->getPhysicsWorld()->getShapes(location);
    PhysicsBody *body = nullptr;
    for (auto &obj: arr) {
        if ((obj->getBody()->getTag() >= DRAGGABLE_MIN_ID)) {
            body = obj->getBody();
            break;
        }
    }

    if (body != nullptr) {
        // Get a nail to fix the selected object, stop them from moving and colliding
        Node *nail = Node::create();
        PhysicsBody *nail_body = PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY);
        nail->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        nail->setPhysicsBody(nail_body);
        nail->getPhysicsBody()->setDynamic(false);
        nail->setPosition(location);
        this->addChild(nail);
        PhysicsJointPin *joint = PhysicsJointPin::construct(nail_body, body, location);
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
    // Remove the released nail
    auto it = _nails.find(touch->getID());
    if (it != _nails.end()) {
        this->removeChild(it->second);
        _nails.erase(it);
    }
}

