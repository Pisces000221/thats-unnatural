#include "SplashScene.h"
#include "AppMacros.h"
using namespace cocos2d;

bool Splash::init(PhysicsWorld *world)
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

    // Test physics
    Vec2 g = world->getGravity();
    CCLOG("default gravity: (%.2f, %.2f)", g.x, g.y);
    world->setGravity(Vec2(25, -90));

    auto body = PhysicsBody::createEdgeBox(size, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(size.width / 2, size.height / 2));
    edgeNode->setPhysicsBody(body);
    this->addChild(edgeNode);

    auto s1 = Sprite::create("CloseNormal.png");
    s1->setPosition(Vec2(size.width * 0.5, size.height * 0.8));
    auto b1 = PhysicsBody::createCircle(s1->getContentSize().width / 2);
    s1->setPhysicsBody(b1);
    this->addChild(s1);

    auto s2 = Sprite::create("CloseNormal.png");
    s2->setPosition(Vec2(size.width * 0.55, size.height * 0.7));
    auto b2 = PhysicsBody::createCircle(s2->getContentSize().width / 2);
    s2->setPhysicsBody(b2);
    this->addChild(s2);

    auto s3 = Sprite::create("CloseNormal.png");
    s3->setPosition(Vec2(size.width * 0.46, size.height * 0.5));
    s3->setScale(2);
    auto b3 = PhysicsBody::createCircle(s3->getContentSize().width);
    s3->setPhysicsBody(b3);
    this->addChild(s3);

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
