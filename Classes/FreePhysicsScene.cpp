#include "algo/indirect_touch.h"
#include "AppMacros.h"
#include "Bricks.h"
#include "widgets/Dashboard.h"

#include "FreePhysicsScene.h"
using namespace cocos2d;

#define HIT_COUNT_MIN_INTERVAL 0.1f

#define DRAGGABLE_MIN_ID 2
#define DATA_IGNORED_CONTACT 0x5f3759df // Uh...
#define BRICK_INIT_Y_OFFSET 30  // A newly generated brick will be height+30 points high
#define CULLING_BOUND -100      // If a brick's Y position is below this, we say bye bye to it

#define DASHBOARD_ZORDER 18906416   // This guy is a super fan of The Three Body Triology.
#define STATIC_BODY_ZORDER 1
#define BRICKS_ZORDER 2
// Allow 0.3 second of detaching of the sensor line
// That is, if the bricks seperated for 0.2s, the line will still be green
const float FreePhysics::LINE_DETACH_MAX_TIME = 0.3f;

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
    this->addChild(tray_node, STATIC_BODY_ZORDER);

    CCLOG("Default gravity: (%.5f, %.5f)", world->getGravity().x, world->getGravity().y);

    // The dashboard
    auto dashboard = Dashboard::create();
    dashboard->setPosition(Vec2(size.width, 0));
    this->addChild(dashboard, DASHBOARD_ZORDER, TAG_DASHBOARD);
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
            // Set the i-th bit to 0 or 1 depend on b
            if (b) _enabledBrickTypes |= 1 << _i;
            else _enabledBrickTypes &= ~(1 << _i);
        }, _enabledBrickTypes & 1 << _i);
    }
    dashboard->addSplitter();
    dashboard->addLabel("BRICKS/SEC");
    dashboard->addSlider(0.5, 10, 5, 0.5, [this](float val) {
        _timeBtwnBrickGen = 1. / val;
    });

    // A sensor line
    _sensorLine = bricks::new_sensorline(Vec2(0, 0), Vec2(size.width, 0));
    _sensorLine->setPositionY(size.height * 0.6);
    // Doesn't collide with anything, but send contact & seperate messages
    _sensorLine->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
    _sensorLine->getPhysicsBody()->setCollisionBitmask(0x0);
    _sensorLine->getPhysicsBody()->setTag(SENSOR_ID);
    this->addChild(_sensorLine, STATIC_BODY_ZORDER);

    indirect_touch::init();
    indirect_touch::start = TRAY_ID;
    indirect_touch::finish = SENSOR_ID;
    // Contact listener
    // http://www.cnblogs.com/JiaoQing/p/3906780.html
    // http://childhood.logdown.com/posts/192612/chipmunk-collision-in-detailed-cocos2dx-filter
    // https://github.com/chukong/cocos-docs/blob/master/manual/framework/native/wiki/physics/zh.md
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(FreePhysics::onContactBegin, this);
    contactListener->onContactSeperate = CC_CALLBACK_1(FreePhysics::onContactSeperate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    // Turn on timer
    this->getScheduler()->schedule([this](float dt) {
        if ((_timeSinceLastBrickGen += dt) > _timeBtwnBrickGen) {
            _timeSinceLastBrickGen -= _timeBtwnBrickGen;
            this->generateBrick();
        }
    }, this, 0, false, "FREEPHYSICS_GEN");

    // Enable touching
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(FreePhysics::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(FreePhysics::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(FreePhysics::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // Auto-culling for bodies
    this->getScheduler()->schedule(
        CC_CALLBACK_1(FreePhysics::autoCullBricks, this),
        this, 0.1, false, "BODIES_AUTO_CULLING");

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

// Contact-related
bool FreePhysics::onContactBegin(PhysicsContact &contact)
{
    if (_useSensor) {
        // Tell i_t that they collided and calculate
        indirect_touch::add_arc(
            contact.getShapeA()->getBody()->getTag(),
            contact.getShapeB()->getBody()->getTag());
        if (!_lineReached && indirect_touch::calculate()) {
            _lineReached = true;
            lineAttach();
        }
    }
    // Is the tray being hit?
    if (_useMoistening &&
            (contact.getShapeA()->getBody()->getTag() == TRAY_ID
            || contact.getShapeB()->getBody()->getTag() == TRAY_ID)) {
        trayHit(contact.getShapeA()->getBody(), contact.getShapeB()->getBody());
    }
    return true;
}

void FreePhysics::onContactSeperate(PhysicsContact &contact)
{
    if (_useSensor) {
        indirect_touch::remove_arc(
            contact.getShapeA()->getBody()->getTag(),
            contact.getShapeB()->getBody()->getTag());
        if (_lineReached && !indirect_touch::calculate()) {
            _lineReached = false;
            lineAboutDetach();
        }
    }
}

void FreePhysics::lineAttach()
{
    this->getScheduler()->unschedule("LINE_DETACH", this);
    bricks::set_brick_colour(_sensorLine, Color3B::GREEN);
}

void FreePhysics::lineAboutDetach()
{
    if (this->getScheduler()->isScheduled("LINE_DETACH", this)) return;
    this->getScheduler()->schedule([this](float dt) {
        this->getScheduler()->unschedule("LINE_DETACH", this);
        bricks::set_brick_colour(_sensorLine, Color3B::RED);
        this->lineDetach();
    }, this, 0, kRepeatForever, LINE_DETACH_MAX_TIME, false, "LINE_DETACH"); 
}

void FreePhysics::trayHit(PhysicsBody *a, PhysicsBody *b)
{
    // One of a and b is the tray, we ensure a is the tray
    if (b->getTag() == TRAY_ID) {
        auto t = a; a = b; b = t;
    }
    auto it = _moistenTime.find(b->getTag());
    _newBrickMoistened = it == _moistenTime.end();
    if (_newBrickMoistened) {
        _moistenTime.insert(std::make_pair<int, clock_t>(b->getTag(), clock()));
        bricks::set_brick_colour(b->getNode(), Color3B::YELLOW);
        _validHitCount = true;
    } else {
        _validHitCount = (float)(clock() - it->second)
            > HIT_COUNT_MIN_INTERVAL * CLOCKS_PER_SEC;
        it->second = clock();
    }
}

// Operations of bricks
void FreePhysics::generateBrick()
{
    auto size = Director::getInstance()->getVisibleSize();
    // Generate a brick with a random shape
    auto obj = bricks::new_random(24, _enabledBrickTypes);
    if (obj == nullptr) return;     // All types are turned off, see dashboard
    obj->setPosition(Vec2(size.width * RAND_0_1, size.height + BRICK_INIT_Y_OFFSET));
    int id;     // Get an ID, either minimum-1 or maximum+1
    if (rand () % 2 && _minID > MIN_BRICK_ID + 1) id = --_minID; else id = ++_maxID;
    obj->getPhysicsBody()->setTag(id);
    obj->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
    obj->getPhysicsBody()->setGroup(BRICKS_GROUP);
    this->addChild(obj, BRICKS_ZORDER);
}

void FreePhysics::autoCullBricks(float dt)
{
    auto world = this->getScene()->getPhysicsWorld();
    const Vector<PhysicsBody *> bodies = world->getAllBodies();
    for (auto body: bodies) {
        if (body->getPosition().y < CULLING_BOUND) {
            // Out of bound. Let's remove this buddy(body)
            if (_useSensor) indirect_touch::remove_all_arcs(body->getTag());
            if (_useMoistening) _moistenTime.erase(body->getTag());
            // Update minimum and maximum IDs
            if (body->getTag() == _minID)
                while (world->getBody(++_minID) == nullptr && _minID < _maxID) {}
            if (body->getTag() == _maxID)
                while (world->getBody(--_maxID) == nullptr && _maxID >= MIN_BRICK_ID) {}
            world->removeBody(body);
        }
    }
    if (MIN_BRICK_ID > _maxID) { _minID = MIN_BRICK_ID; _maxID = MIN_BRICK_ID - 1; }
}

