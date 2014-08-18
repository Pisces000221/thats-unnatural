// The dashboard that is used in FreePhysicsScene

#ifndef __WIDGET_DASHBOARD_H__
#define __WIDGET_DASHBOARD_H__

#include <functional>
#include "cocos2d.h"

class Dashboard : public cocos2d::LayerColor
{
public:
    Dashboard(): _shown(false), _lastPosY(0) { }
    virtual bool init() override;
    CREATE_FUNC(Dashboard);

    void addLabel(std::string text);
    void addGravityPicker(std::function<void(cocos2d::Vec2)>);
    void addTickButton(std::string,
        std::function<void(bool)>, bool checked = false);

protected:
    bool _shown;    // true if expanded
    float _lastPosY;
    inline void updateLastPosY(cocos2d::Node *newChild, bool splitter = true)
    { _lastPosY -= (newChild->getContentSize().height + (splitter ? 24 : 0)); }
};

#endif

