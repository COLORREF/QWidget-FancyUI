//
// Created by TX on 2025/10/23.
//

#include "ScrollBar.h"
#include "ScrollBarStyle.h"

namespace fancy
{
    ScrollBar::ScrollBar(QWidget *parent) :
        QScrollBar(parent)
    {
        setStyle(new ScrollBarStyle(this));
    }

    ScrollBar::ScrollBar(Qt::Orientation ort, QWidget *parent) :
        QScrollBar(ort, parent)
    {
        setStyle(new ScrollBarStyle(this));
    }
} // fancy
