#ifndef CUSTOMCONTROL_H
#define CUSTOMCONTROL_H

#include <QColor>
#include <QMap>
#include <QStyle>
#include "utils/precompile.h"

class ControlColors
{
public:
    static ControlColors* controlColors()
    {
        static ControlColors colors;
        return &colors;
    };
    inline QMap<QString,QColor>& ExpandColors(){return this->_expandColors;};
private:
    // 默认颜色
    ControlColors():
        _disEnabled(180,180,180),
        _normalBorder(131,131,131),
        _prominence(212,78,125),
        _background(Qt::GlobalColor::transparent),
        _text(Qt::GlobalColor::black)
    {};
    QMap<QString,QColor> _expandColors;
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,disEnabled,DisEnabled,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,normalBorder,NormalBorder,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,prominence,Prominence,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,background,Background,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,text,Text,)
};


struct ControlState
{
    ControlState(QStyle::State state)
    {
        enable = state & QStyle::StateFlag::State_Enabled;
        on = state & QStyle::StateFlag::State_On;
        off = state & QStyle::StateFlag::State_Off;
        over = state & QStyle::StateFlag::State_MouseOver;
        sunken = state & QStyle::StateFlag::State_Sunken;
    }
    bool enable;
    bool on ;
    bool off;
    bool over;
    bool sunken;
};



struct CheckableControlState : ControlState
{
    CheckableControlState(QStyle::State state)
        :ControlState(state)
    {
        normal = enable && off && !over && !sunken;
        normal_over = enable && off && over && !sunken;
        normal_sunke = enable && off && over && sunken;
        selected = enable && on && !over && !sunken;
        select_sunken = enable && on && over && sunken;
        select_over = enable && on && over && !sunken;
        unenable = !enable && off;
        unenable_select = !enable && on;
    }
    bool normal;
    bool normal_over;
    bool normal_sunke;
    bool selected;
    bool select_sunken ;
    bool select_over;
    bool unenable;
    bool unenable_select;
};


struct UnCheckableControlState : ControlState
{
    UnCheckableControlState(QStyle::State state)
        :ControlState(state)
    {
        normal = enable && !over && !sunken;
        normal_over = enable && over && !sunken;
        normal_sunke = enable && over && sunken;
        unenable = !enable;
    }
    bool normal;
    bool normal_over;
    bool normal_sunke;
    bool unenable;
};







#endif
