#include "Slider.h"

#include "cocos2d.h"
using namespace cocos2d;

Slider *Slider::create(float min, float max, callback_type callback)
{
    Slider *ret = new Slider();
    if (ret && ret->init(min, max, callback)) return ret;
    else { CC_SAFE_DELETE(ret); ret = nullptr; return nullptr; }
}

bool Slider::init(float min, float max, callback_type callback)
{
    if (!Node::init()) return false;
    _val = _minval = min; _maxval = max; _callback = callback;

    _bar = Sprite::create("images/slider_bar.png");
    _bar->setNormalizedPosition(Vec2(0.5, 0.5));
    _bar->setColor(Color3B(255, 192, 64));
    _bar->setOpacity(96);
    this->addChild(_bar);

    _ptbar = ProgressTimer::create(Sprite::create("images/slider_bar.png"));
    _ptbar->getSprite()->setColor(Color3B(255, 192, 64));
    _ptbar->setNormalizedPosition(Vec2(0.5, 0.5));
    _ptbar->setType(ProgressTimer::Type::BAR);
    _ptbar->setBarChangeRate(Vec2(1, 0));
    _ptbar->setMidpoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(_ptbar);

    _thumb = Sprite::create("images/gpicker_thumb.png");
    _thumb->setPositionY(_contentSize.height * 0.5);
    _thumb->setScale(1.618034);
    this->addChild(_thumb);

    this->setContentSize(_bar->getTextureRect().size);
    refreshDisp();
    return true;
}

void Slider::setContentSize(const Size &newSize)
{
    //CCLOG("%f x %f", newSize.width, newSize.height);
    float w = _bar->getContentSize().width;
    float h = _bar->getContentSize().height;
    Node::setContentSize(Size(newSize.width, h));
    _bar->setScaleX(newSize.width / w);
    _ptbar->setScaleX(newSize.width / w);
    refreshDisp();
}

void Slider::refreshDisp()
{
    float r = (_val - _minval) / (_maxval - _minval);
    _thumb->setPositionX(r * _contentSize.width);
    _ptbar->setPercentage(r * 100.f);
}

