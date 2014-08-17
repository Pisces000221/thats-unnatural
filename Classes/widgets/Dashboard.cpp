#include "AppMacros.h"
#include "widgets/GravityPicker.h"

#include "Dashboard.h"
using namespace cocos2d;

bool Dashboard::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 48))) return false;
    auto size = Director::getInstance()->getVisibleSize();
    this->setContentSize(Size(size.width * 0.5, size.height));
    _lastPosY = size.height - 12;

    // TODO: Make this code cleaner
    auto btn_show = MenuItemImage::create(
        "images/dashboard_btn.png", "images/dashboard_btn.png",
        [this](Ref *sender) {
            CCLOG("oh yeah");
            // Let's go! Move! Move!!
            this->runAction(EaseSineOut::create(
                MoveBy::create(0.5, Vec2(-_contentSize.width, 0))
            ));
            ((Node *)sender)->runAction(FadeOut::create(0.2));
            // Enable tap on blanks to hide
            auto hider_menu = Menu::create();
            hider_menu->setPosition(Vec2::ZERO);
            auto hider_item = MenuItemImage::create(
                "images/white_pixel.png", "images/white_pixel.png",
                [this, hider_menu, sender](Ref *this_sender) {
                    hider_menu->removeFromParent();
                    this->runAction(EaseSineOut::create(
                        MoveBy::create(0.5, Vec2(_contentSize.width, 0))));
                    ((Node *)sender)->runAction(Sequence::create(
                        DelayTime::create(0.3), FadeIn::create(0.2)));
                });
            hider_item->setOpacity(0);
            hider_item->setScale(_contentSize.width, _contentSize.height);
            hider_item->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
            hider_item->setPosition(Vec2::ZERO);
            hider_menu->addChild(hider_item);
            this->addChild(hider_menu);
        });
    btn_show->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    btn_show->setNormalizedPosition(Vec2(0, 0.618034));
    auto menu = Menu::create(btn_show, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}

void Dashboard::addGravityPicker(std::function<void(Vec2)> callback)
{
    auto label = LABEL("GRAVITY", 28, "Light");
    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    label->setPosition(Vec2(0, _lastPosY));
    this->addChild(label);
    updateLastPosY(label, false);
    auto picker = GravityPicker::create();
    picker->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    picker->setPosition(Vec2(_contentSize.width * 0.5, _lastPosY));
    picker->valueChangedCallback = callback;
    this->addChild(picker);
    updateLastPosY(picker, true);
}

