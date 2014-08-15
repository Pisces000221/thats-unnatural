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

#define SCENE_FUNC(__type__) \
static cocos2d::Scene *createScene() { \
    cocos2d::Scene *scene = cocos2d::Scene::create(); \
    __type__ *layer = __type__::create(); \
    scene->addChild(layer); \
    return scene; \
}

// Static create functions
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

// Global function declarations
cocos2d::Label *LABEL(std::string text, int size = 12, 
    std::string style = "", float maxlen = 0);

// Global macro functions
#define RAND_0_1 ((float)rand() / (float)RAND_MAX)

#endif
