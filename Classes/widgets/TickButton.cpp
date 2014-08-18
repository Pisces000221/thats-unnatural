#include "AppMacros.h"

#include "TickButton.h"
using namespace cocos2d;

TickButton *TickButton::create(std::string text, callback_type callback)
{
    TickButton *ret = new TickButton();
    if (ret && ret->init(text, callback)) return ret;
    else { delete ret; ret = nullptr; return nullptr; }
}

bool TickButton::init(std::string text, callback_type callback)
{
    if (!Node::init()) return false;
    _callback = callback;

    // The background
    _bg = Sprite::create("images/white_pixel.png");
    _bg->setScaleY(BUTTON_HEIGHT);  // The width will be set in setContentSize
    _bg->setNormalizedPosition(Vec2(0.5, 0.5));
    _bg->setColor(Color3B(255, 192, 64));
    this->addChild(_bg);

    // The label
    _label = LABEL(text, LABEL_FONTSIZE);
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _label->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(_label);

    // The tick
    _tick = Sprite::create("images/tick.png");
    _tick->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _tick->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    this->addChild(_tick);

    // Enable tapping
    _item = MenuItem::create([this](Ref *sender) { this->toggle(); });
    _item->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    auto menu = Menu::create(_item, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    this->refreshDisp();
    return true;
}

void TickButton::setContentSize(const Size &newSize)
{
    // You just gotta ignore~~ the height~~
    Node::setContentSize(Size(newSize.width, BUTTON_HEIGHT));
    _bg->setScaleX(newSize.width);
    _item->setContentSize(Size(newSize.width, BUTTON_HEIGHT));
}

void TickButton::refreshDisp()
{
    _bg->runAction(FadeTo::create(0.25, _ticked ? 255 : 96));
    _tick->runAction(FadeTo::create(0.25, _ticked ? 255 : 0));
}

