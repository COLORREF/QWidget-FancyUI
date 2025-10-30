//
// Created by TX on 2025/9/19.
//

#include "MinimizeButton.h"

#include "Defs.hpp"
#include "IconEngine.h"
#include "Palette.h"
#include "SvgWidget.h"

namespace fancy
{
    MinimizeButton::MinimizeButton(QWidget *parent) :
        UniversalTitleBarButton(parent)
    {
        svgWidget()->reload(AntDesignIcons::Minus);
        connect(this, &MinimizeButton::clicked, window(), &QWidget::showMinimized);
    }
}

// fancy
