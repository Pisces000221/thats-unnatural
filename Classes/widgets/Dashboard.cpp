#include "AppMacros.h"
#include "widgets/GravityPicker.h"
#include "widgets/TickButton.h"

#include "Dashboard.h"
using namespace cocos2d;

const float Dashboard::NORM_WIDTH = 0.618034f;
bool Dashboard::init()
{
    if (!LayerColor::initWithColor(Color4B(108, 108, 108, 48))) return false;
    auto size = Director::getInstance()->getVisibleSize();
    this->setContentSize(Size(size.width * NORM_WIDTH, size.height));
    _lastPosY = size.height - 12;

    // TODO: Make this code cleaner
    auto btn_show = MenuItemImage::create(
        "images/dashboard_btn.png", "images/dashboard_btn.png",
        [this, size](Ref *sender) {
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
                        DelayTime::create(0.3), FadeIn::create(0.2), nullptr));
                });
            hider_item->setOpacity(0);
            hider_item->setScale(size.width * (1 - NORM_WIDTH), _contentSize.height);
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

void Dashboard::addLabel(std::string text)
{
    auto label = LABEL(text, 28, "Light");
    label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    label->setPosition(Vec2(0, _lastPosY));
    this->addChild(label);
    updateLastPosY(label, false);
}

void Dashboard::addGravityPicker(std::function<void(Vec2)> callback)
{
    auto picker = GravityPicker::create();
    picker->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    picker->setPosition(Vec2(_contentSize.width * 0.5, _lastPosY));
    picker->valueChangedCallback = callback;
    this->addChild(picker);
    updateLastPosY(picker);
}

void Dashboard::addTickButton(std::string text,
    std::function<void(bool)> callback, bool checked)
{
    auto btn = TickButton::create(text, callback);
    btn->setContentSize(_contentSize);  // The height is ignored automatically
    btn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    btn->setPosition(Vec2(0, _lastPosY));
    btn->setTicked(checked);
    this->addChild(btn);
    updateLastPosY(btn, false);
}

