#include "AppMacros.h"
#include "Bricks.h"

#include "LevelListScene.h"
using namespace cocos2d;

#define LEVEL_BRICK_SIDELEN 64
#define LIGHTEN(_c) (128 + (_c).r / 2), (128 + (_c).g / 2), (128 + (_c).b / 2)

const Color3B LevelList::groupColours[] = {
    Color3B(192, 192, 255), Color3B(192, 255, 192),
    Color3B(192, 255, 255), Color3B(255, 192, 192),
    Color3B(255, 192, 255), Color3B(255, 255, 192)
};

LevelList::LevelList()
: _selectedNode(nullptr), _dragStarted(false), _dragStartPosY(0)
{ }

bool LevelList::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    Size size = Director::getInstance()->getVisibleSize();
    CREATE_GO_BACK_MENUITEM;

    _frontLayer = Layer::create();
    this->addChild(_frontLayer);

    // The title
    auto lbl_title = LABEL("LEVELS", 48, "Bold");
    lbl_title->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    lbl_title->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    this->addChild(lbl_title, 10000);

    // The edge
    auto edge_body = PhysicsBody::createEdgeBox(Size(size.width, 1e6), PHYSICSBODY_MATERIAL_DEFAULT, 1);
    auto edge_node = Node::create();
    edge_node->setPosition(Vec2(size.width / 2, 5e5));
    edge_node->setPhysicsBody(edge_body);
    _frontLayer->addChild(edge_node);

    // The levels
    float cur_height = 0;
    for (int i = 0; i < 90; i++) {
        auto brk_level = bricks::new_circle(LEVEL_BRICK_SIDELEN * 0.5);
        bricks::set_brick_colour(brk_level, groupColours[(i / 10) % GROUP_COLOUR_COUNT]);
        cur_height += RAND_DECIMAL(-0.5, 1.5) * LEVEL_BRICK_SIDELEN;
        brk_level->setPosition(Vec2(
            RAND_DECIMAL(LEVEL_BRICK_SIDELEN / 2, size.width - LEVEL_BRICK_SIDELEN / 2),
            cur_height));
        char s[6]; sprintf(s, "%02d", i + 1);
        auto lbl_level = LABEL(s, 32, "Light");
        lbl_level->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        brk_level->addChild(lbl_level, 10); // Keep the number on the top
        _frontLayer->addChild(brk_level);
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
    if (arr.size() == 0) return true;   // Prepare for dragging

    PhysicsBody *body = arr.at(0)->getBody();
    _selectedNode = bricks::get_coloured_part(body->getNode());
    _lastColour = _selectedNode->getColor();
    _selectedNode->runAction(TintTo::create(0.3, LIGHTEN(_lastColour)));
    return true;
}

void LevelList::onTouchMoved(Touch *touch, Event *event)
{
    auto location = touch->getLocation();
    if (_dragStarted) {
        _frontLayer->setPositionY(std::min(0.f, location.y - _dragStartPosY));
    } else if (touch->getStartLocation().getDistance(location) > 10) {
        // Not tapping on a brick now... So start dragging will help, perhaps.
        resetSelected();
        _dragStartPosY = location.y - _frontLayer->getPositionY();
        _dragStarted = true;
    }
}

void LevelList::onTouchEnded(Touch *touch, Event *event)
{
    this->resetSelected();
    _dragStarted = false;
}

void LevelList::resetSelected()
{
    if (_selectedNode) {
        _selectedNode->runAction(TintTo::create(0.3,
            _lastColour.r, _lastColour.g, _lastColour.b));
        _selectedNode = nullptr;
    }
}

