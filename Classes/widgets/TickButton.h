#ifndef __WIDGET_TICK_BUTTON_H__
#define __WIDGET_TICK_BUTTON_H__

#include "cocos2d.h"
#include <functional>
#include <string>

#define EXEC_CALLBACK if (_callback) _callback(_ticked)
class TickButton : public cocos2d::Node
{
public:
    TickButton() : _ticked(false) {}
    typedef std::function<void(bool)> callback_type;
    virtual bool init(std::string, callback_type);
    virtual void setContentSize(const cocos2d::Size &) override;
    inline void setTicked(bool ticked) { _ticked = ticked; refreshDisp(); EXEC_CALLBACK; }
    inline void toggle() { _ticked = !_ticked; refreshDisp(); EXEC_CALLBACK; }
    inline bool getTicked() { return _ticked; }
    static TickButton *create(std::string, callback_type);

    static const int BUTTON_HEIGHT = 32;
    static const int LABEL_FONTSIZE = 28;

protected:
    callback_type _callback;
    bool _ticked;
    cocos2d::Sprite *_bg;
    cocos2d::Sprite *_tick;
    cocos2d::Label *_label;
    cocos2d::MenuItem *_item;

    void refreshDisp();
};
#undef EXEC_CALLBACK

#endif

