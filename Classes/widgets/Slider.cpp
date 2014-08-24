#include "Slider.h"

#include "cocos2d.h"
using namespace cocos2d;

Slider *Slider::create(float min, float max, float increment, callback_type callback)
{
    Slider *ret = new Slider();
    if (ret && ret->init(min, max, increment, callback)) return ret;
    else { CC_SAFE_DELETE(ret); ret = nullptr; return nullptr; }
}

bool Slider::init(float min, float max, float increment, callback_type callback)
{
    if (!Node::init()) return false;
    _val = _minval = min; _maxval = max; _increment = increment;
    _callback = callback;
    // Calculate number format, currently only decimal precision supported
    int digits = 0;
    // Use increment as a temporary variable
    while (fabs((int)increment - increment) / _increment > 1e-5) {
        increment *= 10.; ++digits;
    }
    _num_format = new char[10]; sprintf(_num_format, "%%.%df", digits);

    _bar = Sprite::create("images/slider_bar.png");
    _bar->setNormalizedPosition(Vec2(0.5, 0));
    _bar->setColor(Color3B(255, 192, 64));
    _bar->setOpacity(96);
    this->addChild(_bar);

    _ptbar = ProgressTimer::create(Sprite::create("images/slider_bar.png"));
    _ptbar->getSprite()->setColor(Color3B(255, 192, 64));
    _ptbar->setNormalizedPosition(Vec2(0.5, 0));
    _ptbar->setType(ProgressTimer::Type::BAR);
    _ptbar->setBarChangeRate(Vec2(1, 0));
    _ptbar->setMidpoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(_ptbar);

    _thumb = Sprite::create("images/gpicker_thumb.png");
    _thumb->setPositionY(_contentSize.height * 0.5);
    _thumb->setScale(1.618034);
    this->addChild(_thumb);

    _label = LABEL("", 24, "Light");
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _label->setNormalizedPosition(Vec2(0.5, 0));
    _label->setScale(0.5);  // Prevent over-antialiasing
    _thumb->addChild(_label);

    // Enable touching
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Slider::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Slider::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Slider::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->setContentSize(Size(_bar->getContentSize().width,
        _thumb->getContentSize().height + _label->getContentSize().height * 0.5));
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

void Slider::setValue(float val)
{
    _val = val;
    ENSURE_IN_RANGE(_val, _minval, _maxval);
    refreshDisp();
    if (_callback) _callback(_val);
}

bool Slider::onTouchBegan(Touch *touch, Event *event)
{
    if (!Rect(0, 0, _contentSize.width, _contentSize.height)
        .containsPoint(this->convertTouchToNodeSpace(touch))) return false;
    this->onTouchMoved(touch, event);
    return true;
}

void Slider::onTouchMoved(Touch *touch, Event *event)
{
    float x = this->convertTouchToNodeSpace(touch).x;
    this->setValueByRate(x / _contentSize.width);
}

void Slider::onTouchEnded(Touch *touch, Event *event)
{
}

void Slider::refreshDisp()
{
    float r = (_val - _minval) / (_maxval - _minval);
    _thumb->setPositionX(r * _contentSize.width);
    _ptbar->setPercentage(r * 100.f);
    char s[5]; sprintf(s, _num_format, _val);
    _label->setString(s);
}

