//
// Created by TX on 2025/10/6.
//

#include "ScrollAreaStyle.h"

#include <QScrollArea>

namespace fancy
{
    ScrollAreaStyle::ScrollAreaStyle(QScrollArea *parent) :
        QProxyStyle(nullptr)
    {
        setParent(parent);
    }

    void ScrollAreaStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        return;
    }

    void ScrollAreaStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        return;
    }
} // fancy
