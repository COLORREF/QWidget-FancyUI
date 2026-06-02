//
// Created by TX on 2025/10/16.
//

#ifndef QWIDGET_FANCYUI_SCROLLBARSTYLE_H
#define QWIDGET_FANCYUI_SCROLLBARSTYLE_H
#include <QProxyStyle>

class QScrollBar;

namespace fancy
{
    enum class Direction;

    class ScrollBarStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit ScrollBarStyle(QScrollBar *parent);

    protected:
        void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const override;

        int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override;

        static void drawArrow(QPainter *painter, const QRectF &arrowRect, Direction dir);
    };
} // fancy

#endif //QWIDGET_FANCYUI_SCROLLBARSTYLE_H
