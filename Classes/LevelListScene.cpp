#include "AppMacros.h"
#include "Bricks.h"

#include "LevelListScene.h"
using namespace cocos2d;

#define LEVEL_BRICK_SIDELEN 64

bool LevelList::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    Size size = Director::getInstance()->getVisibleSize();
    CREATE_GO_BACK_MENUITEM;

    // The title
    auto lbl_title = LABEL("LEVELS", 48, "Bold");
    lbl_title->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    lbl_title->setPosition(Vec2::ANCHOR_TOP_RIGHT);
    this->addChild(lbl_title);

    // The edge
    auto edge_body = PhysicsBody::createEdgeBox(size, PHYSICSBODY_MATERIAL_DEFAULT, 1);
    auto edge_node = Node::create();
    edge_node->setPosition(Vec2(size.width / 2, size.height / 2));
    edge_node->setPhysicsBody(edge_body);
    this->addChild(edge_node);

    // The levels
    float cur_height = 0;
    for (int i = 0; i < 10; i++) {
        auto brk_level = bricks::new_rect(LEVEL_BRICK_SIDELEN, LEVEL_BRICK_SIDELEN);
        cur_height += RAND_DECIMAL(-0.5, 1.5) * LEVEL_BRICK_SIDELEN;
        brk_level->setPosition(Vec2(
            RAND_DECIMAL(LEVEL_BRICK_SIDELEN / 2, size.width - LEVEL_BRICK_SIDELEN / 2),
            cur_height));
        char s[6]; sprintf(s, "%02d", i + 1);
        auto lbl_level = LABEL(s, 32, "Bold");
        lbl_level->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        brk_level->addChild(lbl_level, 10); // Keep the number on the top
        this->addChild(brk_level);
    }

    // Enable touching
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(LevelList::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LevelList::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LevelList::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

// Enable dragging & selecting
bool LevelList::onTouchBegan(Touch* touch, Event* event)
{
    auto location = touch->getLocation();
    auto arr = this->getScene()->getPhysicsWorld()->getShapes(location);
    if (arr.size() == 0) return false;

    PhysicsBody *body = arr.at(0)->getBody();
    Node *node = body->getNode();
    bricks::get_coloured_part(node)->runAction(TintTo::create(0.3, 216, 216, 255));
    return true;
}

void LevelList::onTouchMoved(Touch *touch, Event *event)
{
}

void LevelList::onTouchEnded(Touch *touch, Event *event)
{
}

