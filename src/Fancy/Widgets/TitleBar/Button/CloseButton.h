//
// Created by TX on 2025/9/19.
//

#ifndef QWIDGET_FANCYUI_CLOSEBUTTON_H
#define QWIDGET_FANCYUI_CLOSEBUTTON_H

#include "UniversalTitleBarButton.h"

namespace fancy
{
    enum class TitleBtnState;
    class SvgWidget;

    class CloseButton : public UniversalTitleBarButton
    {
        Q_OBJECT
        friend class TitleBar;
        friend class Window;

    public:
        explicit CloseButton(QWidget *parent);

    protected:
        [[nodiscard]] QBrush maskBrush() const override;

        void enterEvent(QEnterEvent *event) override;

        void leaveEvent(QEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void paintEvent(QPaintEvent *event) override;
    };
} // fancy

#endif //QWIDGET_FANCYUI_CLOSEBUTTON_H
