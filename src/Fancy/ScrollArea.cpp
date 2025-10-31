//
// Created by TX on 2025/10/6.
//

#include "ScrollArea.h"

#include "ScrollAreaStyle.h"
#include "ScrollBar.h"

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

        setVerticalScrollBar(new ScrollBar(Qt::Orientation::Vertical, this));
        setHorizontalScrollBar(new ScrollBar(Qt::Orientation::Horizontal, this));
    }
} // fancy
