#ifndef __WIDGETS_DIALOGUE_H__
#define __WIDGETS_DIALOGUE_H__

#include "cocos2d.h"
#include <string>

class Dialogue : public cocos2d::Layer
{
public:
    Dialogue() {}
    virtual bool init();
    CREATE_FUNC(Dialogue);
    void setTitle(std::string title) { _lblTitle->setString(title); }
    void setMessage(std::string msg) { _lblMsg->setString(msg); }

protected:
    cocos2d::Label *_lblTitle, *_lblMsg;
};

#endif

