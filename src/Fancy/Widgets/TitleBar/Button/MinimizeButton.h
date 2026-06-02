//
// Created by TX on 2025/9/19.
//

#ifndef QWIDGET_FANCYUI_MINIMIZEBUTTON_H
#define QWIDGET_FANCYUI_MINIMIZEBUTTON_H
#include "UniversalTitleBarButton.h"

namespace fancy
{
    enum class TitleBtnState;
    class SvgWidget;

    class MinimizeButton : public UniversalTitleBarButton
    {
        Q_OBJECT

        friend class TitleBar;
        friend class Window;

    public:
        explicit MinimizeButton(QWidget *parent);
    };
} // fancy

#endif //QWIDGET_FANCYUI_MINIMIZEBUTTON_H
