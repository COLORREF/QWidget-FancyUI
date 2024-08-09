#ifndef CONTROLCOLORS_H
#define CONTROLCOLORS_H

#include <QColor>
#include "utils/precompile.h"

class ControlColors
{
public:
    static ControlColors* controlColors()
    {
        static ControlColors colors;
        return &colors;
    };
private:
    // 默认颜色
    ControlColors():
        _disEnabled(180,180,180),
        _normalBorder(131,131,131),
        _prominence(212,78,125),
        _background(Qt::GlobalColor::transparent),
        _text(Qt::GlobalColor::black)
    {};
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,disEnabled,DisEnabled,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,normalBorder,NormalBorder,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,prominence,Prominence,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,background,Background,)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,text,Text,)
};

#endif // CONTROLCOLORS_H
