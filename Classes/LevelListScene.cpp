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

    return true;
}

