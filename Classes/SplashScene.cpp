#include "SplashScene.h"
#include "AppMacros.h"
using namespace cocos2d;

bool Splash::init()
{
    if (!Layer::init()) return false;

    Size size = Director::getInstance()->getVisibleSize();

    auto bgSprite = Sprite::create("Default.png");
    bgSprite->setNormalizedPosition(Vec2(0.5, 0.5));
    bgSprite->setScaleX(size.width / bgSprite->getContentSize().width);
    bgSprite->setScaleY(size.height / bgSprite->getContentSize().height);
    this->addChild(bgSprite);

    auto label = LabelTTF::create("Hello World", "Arial", 54);
    label->setPosition(
        Vec2(size.width / 2, size.height - label->getContentSize().height));
    this->addChild(label);

    auto closeItem = MenuItemImage::create(
        "CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(Splash::close, this));
    closeItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    closeItem->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}


void Splash::close(Ref *sender)
{
#if IS_WINDOWS_PHONE
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();
#if IS_IOS_DEVICE
    exit(0);
#endif
}
