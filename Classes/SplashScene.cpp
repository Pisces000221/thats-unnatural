#include "SplashScene.h"
#include "AppMacros.h"
using namespace cocos2d;

bool Splash::init()
{
    if (!LayerColor::initWithColor(Color4B::WHITE)) return false;

    Size size = Director::getInstance()->getVisibleSize();

    auto cocos_logo = Sprite::create("cocos2dx_portrait.png");
    cocos_logo->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    cocos_logo->setPosition(Vec2(size.width + 30, size.height + 30));
    cocos_logo->setScale(size.width * 0.9 / cocos_logo->getContentSize().width);
    cocos_logo->setOpacity(108);
    this->addChild(cocos_logo);

    auto lbl_powered = LABEL("Powered by", 36, "Italic.leftalign");
    lbl_powered->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbl_powered->setNormalizedPosition(Vec2(0, 0.3));
    this->addChild(lbl_powered);

    auto lbl_cocos2dx = LABEL("Cocos2d-x", 52, "BoldItalic.leftalign");
    lbl_cocos2dx->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    lbl_cocos2dx->setNormalizedPosition(Vec2(0, 0.15));
    this->addChild(lbl_cocos2dx);

    return true;
}

