//
// Created by TX on 2025/10/6.
//

#include "ScrollArea.h"

#include "ScrollAreaStyle.h"

namespace fancy
{
    ScrollArea::ScrollArea(QWidget *parent) :
        QScrollArea(parent)
    {
        QPalette p = palette();
        p.setColor(QPalette::Base, Qt::transparent);
        p.setColor(QPalette::Window, Qt::transparent);
        setPalette(p);
        setStyle(new ScrollAreaStyle(this));
    }
} // fancy
