#include "AppMacros.h"
#include "Bricks.h"
#include "data/level_reader.h"
#include "LevelPlayScene.h"

#include "LevelListScene.h"
using namespace cocos2d;

static LevelList *s_sharedLevelList;
LevelList *LevelList::getInstance()
{
    if (!s_sharedLevelList) auto scene = LevelList::createScene();
    return s_sharedLevelList;
}

#define LEVEL_BRICK_SIDELEN 64
#define LIGHTEN(_c) (128 + (_c).r / 2), (128 + (_c).g / 2), (128 + (_c).b / 2)

const Color3B LevelList::groupColours[] = {
    Color3B(192, 192, 255), Color3B(192, 255, 192),
    Color3B(192, 255, 255), Color3B(255, 192, 192),
    Color3B(255, 192, 255), Color3B(255, 255, 192)
};

// http://blog.chinaunix.net/uid-17019762-id-3152012.html
LevelList::LevelList()
: _levelsDisplayed(0), _maxHeight(0.0f), _lastLevelBrick(nullptr),
  _selectedNode(nullptr), _selectedLevel(0),
  _dragStarted(false), _dragStartPosY(0)
{ }

bool LevelList::init(PhysicsWorld *world)
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;
    s_sharedLevelList = this;
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
    CCLOG("%d", _levelsDisplayed);
    int initialLevels = UserDefault::getInstance()->getIntegerForKey("levels_cleared", 0);
    for (int i = 0; i <= initialLevels; i++) this->addLevel(false);
    CCLOG("aha");

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
    _selectedLevel = body->getNode()->getTag() - TAG_LEVEL_0 + 1;
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
    if (!_dragStarted && _selectedLevel != 0) enterLevel(_selectedLevel);
    _dragStarted = false; _selectedLevel = 0;
}

void LevelList::addLevel(bool appending)
{
    auto size = Director::getInstance()->getVisibleSize();
    float cur_height = appending ?
        -_frontLayer->getPosition().y + size.height + LEVEL_BRICK_SIDELEN * 2
        : _maxHeight;
    int i = _levelsDisplayed++;
    auto brk_level = bricks::new_circle(LEVEL_BRICK_SIDELEN * 0.5);
    if (!appending) _maxHeight += RAND_DECIMAL(-0.5, 1.5) * LEVEL_BRICK_SIDELEN;
    ENSURE_IN_RANGE(cur_height, LEVEL_BRICK_SIDELEN * 1.5, 1e6);
    brk_level->setPosition(Vec2(
        RAND_DECIMAL(LEVEL_BRICK_SIDELEN / 2, size.width - LEVEL_BRICK_SIDELEN / 2),
        cur_height));
    char s[6]; sprintf(s, "%02d", i + 1);
    auto lbl_level = LABEL(s, 32, "Light");
    lbl_level->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    brk_level->addChild(lbl_level, 10); // Keep the number on the top
    brk_level->setTag(i + TAG_LEVEL_0);
    _frontLayer->addChild(brk_level);
    // Bring the colour of the last brick back
    bricks::set_brick_colour(brk_level, Color3B::WHITE);
    if (_lastLevelBrick)
        bricks::set_brick_colour(_lastLevelBrick,
            groupColours[((i - 1) / 10) % GROUP_COLOUR_COUNT]);
    _lastLevelBrick = brk_level;
}

void LevelList::resetSelected()
{
    if (_selectedNode) {
        _selectedNode->runAction(TintTo::create(0.3,
            _lastColour.r, _lastColour.g, _lastColour.b));
        _selectedNode = nullptr;
    }
}

void LevelList::enterLevel(int id)
{
    CCLOG("Enter level %d", id);
    LevelPlay::setLevelToLoad(level_reader::get_level(id));
    GO_TO_SCENE(LevelPlay);
}

