#ifndef __MACROS_H__
#define __MACROS_H__

#include "cocos2d.h"
#include <string>

// Application Settings
#define IS_USING_SOUND 1

#define IS_WINDOWS_PHONE \
    (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || \
    (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)

#define IS_IOS_DEVICE (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

// Application 'variables'
#define HEIGHT_RATIO (Director::getInstance()->getVisibleSize().height / 480.0)

// Static create functions
#define SCENE_FUNC(__type__) \
static cocos2d::Scene *createScene() { \
    cocos2d::Scene *scene = cocos2d::Scene::create(); \
    __type__ *layer = __type__::create(); \
    scene->addChild(layer); \
    return scene; \
}

#define PHY_CREATE_FUNC(__type__) \
static __type__ *create(cocos2d::PhysicsWorld *world) { \
    __type__ *ret = new __type__; \
    if (ret && ret->init(world)) { \
        ret->autorelease(); \
        return ret; \
    } else { \
        delete ret; \
        ret = nullptr; return nullptr; \
    } \
}

#define PHY_SCENE_FUNC(__type__) \
static cocos2d::Scene *createScene() { \
    cocos2d::Scene *scene = cocos2d::Scene::createWithPhysics(); \
    __type__ *layer = __type__::create(scene->getPhysicsWorld()); \
    scene->addChild(layer); \
    return scene; \
}

// Scene transition related macros
// Requires cocos2d.h and using namespace cocos2d
// Runs an animation to go to the next scene
#define GO_TO_SCENE(__layer_type__) do { \
    Scene *nextScene = __layer_type__::createScene(); \
    Director::getInstance()->pushScene(TransitionFade::create(0.8, nextScene, Color3B::WHITE)); \
    LayerColor *cover = LayerColor::create(Color4B::WHITE); \
    cover->setOpacity(0); \
    this->addChild(cover, INT_MAX); \
    cover->runAction(Sequence::create( \
        DelayTime::create(0.8), \
        CallFunc::create([cover]() { cover->setOpacity(255); }), \
        FadeOut::create(0.4), \
        RemoveSelf::create(), nullptr)); } while (0)
// Runs an animation and go back
#define CREATE_GO_BACK_MENUITEM do { \
    MenuItemImage *_item = MenuItemImage::create( \
        "images/back.png", "images/back.png", \
        [this](Ref *sender) { GO_BACK_ANIMATED; }); \
    _item->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT); \
    _item->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT); \
    _item->setScale((HEIGHT_RATIO - 1) * 0.4 + 1); \
    Menu *_menu = Menu::create(_item, nullptr); \
    _menu->setPosition(Vec2::ZERO); \
    this->addChild(_menu, INT_MAX - 10); } while (0)
// Create a go back method. Can be used without using namespace cocos2d
#define GO_BACK_ANIMATED do { \
    cocos2d::LayerColor *cover = cocos2d::LayerColor::create(cocos2d::Color4B::WHITE); \
    this->getScene()->addChild(cover, INT_MAX); \
    cover->setOpacity(0); \
    cover->runAction(cocos2d::Sequence::create( \
        cocos2d::FadeIn::create(0.4), \
        cocos2d::CallFunc::create([]() { cocos2d::Director::getInstance()->popScene(); }), nullptr)); } while (0)

// Global function declarations
cocos2d::Label *LABEL(std::string text, int size = 12, 
    std::string style = "", float maxlen = 0);

// Global macro functions
#define RAND_0_1 ((float)rand() / (float)RAND_MAX)
#define RAND_DECIMAL(lo, hi) (lo + RAND_0_1 * (hi - lo))

#endif

