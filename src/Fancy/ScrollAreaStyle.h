//
// Created by TX on 2025/10/6.
//

#ifndef QWIDGET_FANCYUI_SCROLLAREASTYLE_H
#define QWIDGET_FANCYUI_SCROLLAREASTYLE_H

#include <QProxyStyle>

class QScrollArea;

namespace fancy
{
    class ScrollAreaStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit ScrollAreaStyle(QScrollArea *parent);

    protected:
        void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;
    };
} // fancy

#endif //QWIDGET_FANCYUI_SCROLLAREASTYLE_H
