#ifndef __MACROS_H__
#define __MACROS_H__

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

#endif
