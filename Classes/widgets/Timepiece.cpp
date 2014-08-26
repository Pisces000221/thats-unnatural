#include "Timepiece.h"
using namespace cocos2d;

Timepiece *Timepiece::create(const Color4B &c, float diametre)
{
    auto t = new Timepiece();
    if (t && t->init(c, diametre)) return t;
    else { if (t) delete t; t = nullptr; return nullptr; }
}

bool Timepiece::init(const Color4B &c, float diametre)
{
    _diametre = diametre;
    auto spr = Sprite::create("images/circle.png");
    spr->setColor(Color3B(c.r, c.g, c.b));
    spr->setOpacity(c.a);
    if (!ProgressTimer::initWithSprite(spr)) return false;
    this->setType(ProgressTimer::Type::RADIAL);
    this->setReverseDirection(true);
    this->setPercentage(100);
    this->setScale(diametre / spr->getContentSize().width);
    return true;
}

void Timepiece::setTime(float rate)
{
    this->setPercentage(100 * rate);
}

