#include "Dialogue.h"
#include "AppMacros.h"
using namespace cocos2d;

bool Dialogue::init()
{
    if (!Layer::init()) return false;
    auto size = Director::getInstance()->getVisibleSize();

    auto bg = Sprite::create("images/white_pixel.png");
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    bg->setPosition(Vec2::ZERO);
    bg->setScale(size.width, size.height);
    bg->setOpacity(0);
    this->addChild(bg, 0);
    bg->runAction(FadeTo::create(1, 192));

    _lblTitle = LABEL("", 60, "Regular");
    _lblTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _lblTitle->setNormalizedPosition(Vec2(0.5, 0.618));
    this->addChild(_lblTitle);

    _lblMsg = LABEL("", 48, "Light");
    _lblMsg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _lblMsg->setNormalizedPosition(Vec2(0.5, 0.618));
    this->addChild(_lblMsg);

    // Eat those touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch *touch, Event *event) { return true; };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

