#include "Dialogue.h"
#include "AppMacros.h"
#include "RetriableProtocol.h"
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

    auto img_back = Sprite::create("images/back.png");
    img_back->setAnchorPoint(Vec2(0.5, 0.4));
    img_back->setNormalizedPosition(Vec2(0.2, 0.3));
    this->addChild(img_back);
    auto item_back = MenuItemLabel::create(
        LABEL("RETURN", 48, "Bold", Color3B::GREEN),
        [this](Ref *sender) { GO_BACK_ANIMATED; }
    );
    item_back->setNormalizedPosition(Vec2(0.6, 0.3));
    this->addChild(MENU(item_back));
    auto img_retry = Sprite::create("images/retry.png");
    img_retry->setAnchorPoint(Vec2(0.5, 0.4));
    img_retry->setNormalizedPosition(Vec2(0.8, 0.2));
    this->addChild(img_retry);
    auto item_retry = MenuItemLabel::create(
        LABEL("RETRY", 48, "Bold", Color3B(128, 255, 255)),
        [this](Ref *sender) {
            auto p = dynamic_cast<RetriableProtocol *>(this->getParent());
            if (p) p->retry();
            else GO_BACK_ANIMATED;
            if (this->getParent()) this->removeFromParent();
        }
    );
    item_retry->setNormalizedPosition(Vec2(0.3, 0.2));
    this->addChild(MENU(item_retry));

    // Eat those touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch *touch, Event *event) { return true; };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

