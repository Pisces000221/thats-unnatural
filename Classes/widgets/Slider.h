#ifndef __WIDGETS_SLIDER_H__
#define __WIDGETS_SLIDER_H__

#include "cocos2d.h"
#include <functional>

#define EXEC_CALLBACK if (_callback) _callback(_val)
class Slider : public cocos2d::Node
{
public:
    Slider() : _callback(nullptr) { }
    typedef std::function<void(float)> callback_type;
    virtual bool init(float, float, callback_type);
    virtual void setContentSize(const cocos2d::Size &) override;
    void setValue(float val) { _val = val; refreshDisp(); EXEC_CALLBACK; }
    float getValue() { return _val; }
    static Slider *create(float, float, callback_type);

protected:
    callback_type _callback;
    float _minval, _maxval, _val;
    cocos2d::Sprite *_bar, *_thumb;
    cocos2d::ProgressTimer *_ptbar;
    cocos2d::Label *_label;

    bool onTouchBegan(cocos2d::Touch *, cocos2d::Event *);
    void onTouchMoved(cocos2d::Touch *, cocos2d::Event *);
    void onTouchEnded(cocos2d::Touch *, cocos2d::Event *);
    void refreshDisp();
};
#undef EXEC_CALLBACK

#endif

