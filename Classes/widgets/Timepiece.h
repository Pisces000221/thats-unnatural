#ifndef __WIDGETS_TIMEPIECE_H__
#define __WIDGETS_TIMEPIECE_H__

#include "cocos2d.h"

class Timepiece : public cocos2d::ProgressTimer
{
public:
    Timepiece() { }
    virtual bool init(const cocos2d::Color4B &c, float diametre);
    static Timepiece *create(const cocos2d::Color4B &c, float diametre);

    float getDiametre() { return _diametre; }
    void setTime(float rate);   // Rate in [0, 1]

protected:
    float _diametre;
    // This is defined in cocos2d::ProgressTimer
    //cocos2d::Sprite *_sprite;
};

#endif

