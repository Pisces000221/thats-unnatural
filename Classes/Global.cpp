#include "AppMacros.h"

cocos2d::Label *LABEL(std::string text, int size,
    std::string style, cocos2d::Color3B colour, float maxlen)
{
    unsigned int p = style.find_first_of(".");
    cocos2d::TextHAlignment align = cocos2d::TextHAlignment::CENTER;
    if (p != std::string::npos) {
        std::string alignment_str = style.substr(p + 1);
        if (alignment_str == "leftalign") {
            align = cocos2d::TextHAlignment::LEFT;
        } else if (alignment_str == "centeralign" || alignment_str == "centrealign") {
            align = cocos2d::TextHAlignment::CENTER;
        } else if (alignment_str == "rightalign") {
            align = cocos2d::TextHAlignment::RIGHT;
        }
        style = style.substr(0, p);
        //CCLOG("%s %s", style.c_str(), alignment_str.c_str());
    }
    if (style == "") style = "Regular";
    cocos2d::Label *label = cocos2d::Label::createWithTTF(
        cocos2d::TTFConfig(("fonts/JosefinSans-" + style + ".ttf").c_str(), size),
        text, align, maxlen);
    label->setColor(colour);
    return label;
}

cocos2d::Menu *MENU(cocos2d::MenuItem *single_item)
{
    cocos2d::Menu *menu = cocos2d::Menu::create();
    menu->addChild(single_item);
    menu->setPosition(cocos2d::Vec2::ZERO);
    return menu;
}

