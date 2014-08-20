#include <cstdlib>

#include "AppDelegate.h"
#include "AppMacros.h"
#include "SplashScene.h"
using namespace cocos2d;

#if IS_USING_SOUND
    #include "SimpleAudioEngine.h"
#endif

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    srand((unsigned)::time(NULL));  // Initialize random generator
    // Initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLView::createWithRect("That's Unnatural!", Rect(0, 0, 320, 480));
        director->setOpenGLView(glview);
    }
    glview->setDesignResolutionSize(320, 480, ResolutionPolicy::FIXED_WIDTH);
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

    // Create a scene
    auto scene = Splash::createScene();
    // Here we go!!
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
#if IS_USING_SOUND
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
#if IS_USING_SOUND
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#endif
}
