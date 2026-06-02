//
// Created by TX on 2025/9/19.
//

#include "MinimizeButton.h"

#include "Core/Defs.h"
#include "Core/Palette/Palette.h"
#include "Icon/IconEngine.h"
#include "Widgets/SvgWidget/SvgWidget.h"

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
