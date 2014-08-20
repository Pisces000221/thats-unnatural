#include <math.h>
#include "AppMacros.h"

#include "GravityPicker.h"
using namespace cocos2d;

#define GP_RADIUS (_contentSize.height)
#define GP_ORIGIN Vec2(_contentSize.width / 2, _contentSize.height)

GravityPicker::GravityPicker()
: _thumb(nullptr), _line(nullptr), valueChangedCallback(nullptr)
{ }

bool GravityPicker::init()
{
    if (!Node::init()) return false;

    auto bg = Sprite::create("images/gpicker.png");
    bg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    this->addChild(bg);
    this->setContentSize(bg->getContentSize());

    _thumb = Sprite::create("images/gpicker_thumb.png");
    _thumb->setPosition(GP_ORIGIN - Vec2(0, GP_RADIUS * 0.5));
    this->addChild(_thumb);

    _line = Sprite::create("images/white_pixel.png");
    _line->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _line->setColor(Color3B(255, 192, 64));
    _line->setPosition(GP_ORIGIN);
    _line->setScaleX(GP_RADIUS * 0.5);
    _line->setRotation(90);
    this->addChild(_line);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, listener](Touch *touch, Event *event) {
        auto p = this->convertTouchToNodeSpace(touch);
        if (this->inRange(p) == 0) {
            // Touch is inside the grey area
            listener->onTouchMoved(touch, nullptr);
            return true;
        } else return false;
    };
    listener->onTouchMoved = [this](Touch *touch, Event *event) {
        auto p = this->convertTouchToNodeSpace(touch);
        double angle = this->getAngle(p);
        if (this->inRange(p) == 1) {
            p = -Vec2(cos(angle), sin(angle)) * GP_RADIUS + GP_ORIGIN;
        } else if (this->inRange(p) == 2) {
            p.y = GP_RADIUS;
            ENSURE_IN_RANGE(p.x, 0, 2 * GP_RADIUS);
            angle = p.x > GP_RADIUS ? M_PI : 0;
        }
        _thumb->setPosition(p);
        _line->setScaleX(GP_ORIGIN.getDistance(p));
        _line->setRotation(180.0 - angle * 180.0 / M_PI);
        if (valueChangedCallback) {
            valueChangedCallback((p - GP_ORIGIN) / GP_RADIUS * 98.000 * 2);
        }
    };
    listener->onTouchEnded = [this](Touch *touch, Event *event) {
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

// Return codes:
// 0: totally in range
// 1: can be normalized by angle (e.g. (0, 0) -> 45 deg)
// 2: can be normalized by x value (e.g. (100, height+123) -> (100, height))
int GravityPicker::inRange(Vec2 &p)
{
    if (p.y <= GP_RADIUS) {
        return p.getDistance(GP_ORIGIN) <= GP_RADIUS ? 0 : 1;
    } else {
        return 2;
    }
}

double GravityPicker::getAngle(Vec2 &p)
{
    Vec2 delta = GP_ORIGIN - p;
    return atan2(delta.y, delta.x);
}

#undef GP_RADIUS
#undef GP_ORIGIN

