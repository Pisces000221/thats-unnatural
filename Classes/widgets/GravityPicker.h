#ifndef __WIDGET_GRAVITY_PICKER_H__
#define __WIDGET_GRAVITY_PICKER_H__

#include "cocos2d.h"
#include <functional>

class GravityPicker : public cocos2d::Node
{
public:
    GravityPicker();
    virtual bool init() override;
    CREATE_FUNC(GravityPicker);
    std::function<void(cocos2d::Vec2)> valueChangedCallback;

protected:
    cocos2d::Sprite *_thumb;
    cocos2d::Sprite *_line;
    inline int inRange(cocos2d::Vec2 &);    // About the return codes, see the .cpp file
    inline double getAngle(cocos2d::Vec2 &);
};

#endif

