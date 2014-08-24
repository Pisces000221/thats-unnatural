#ifndef __WIDGETS_SLIDER_H__
#define __WIDGETS_SLIDER_H__

#include "cocos2d.h"
#include <functional>
#include "AppMacros.h"

class Slider : public cocos2d::Node
{
public:
    Slider() : _callback(nullptr) { }
    ~Slider() { delete _num_format; }
    typedef std::function<void(float)> callback_type;
    virtual bool init(float, float, float, callback_type);
    virtual void setContentSize(const cocos2d::Size &) override;
    void setValue(const float);
    void setValueByRate(const float r) { setValue(_minval + r * (_maxval - _minval)); }
    float getValue() { return _val; }
    static Slider *create(float, float, float, callback_type);

protected:
    callback_type _callback;
    float _minval, _maxval, _val, _increment;
    char *_num_format;
    cocos2d::Sprite *_bar, *_thumb;
    cocos2d::ProgressTimer *_ptbar;
    cocos2d::Label *_label;
    cocos2d::Rect _validTouchRect;

    bool onTouchBegan(cocos2d::Touch *, cocos2d::Event *);
    void onTouchMoved(cocos2d::Touch *, cocos2d::Event *);
    void onTouchEnded(cocos2d::Touch *, cocos2d::Event *);
    void refreshDisp();
};

#endif

