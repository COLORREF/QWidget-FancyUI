//
// Created by TX on 2025/9/26.
//

#ifndef QWIDGET_FANCYUI_SLIDERSTYLE_H
#define QWIDGET_FANCYUI_SLIDERSTYLE_H


#include <QProxyStyle>


class QSlider;

namespace fancy
{
    class SliderIndicator;

    class SliderStyle : public QProxyStyle
    {
    public:
        explicit SliderStyle(QSlider *parent, Qt::Orientation orientation = Qt::Orientation::Horizontal);

        void setOrientation(Qt::Orientation orientation);

        bool eventFilter(QObject *obj, QEvent *event) override;

    protected:
        void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const override;

    private:
        SliderIndicator *_indicator = nullptr;
    };
} // fancy

#endif //QWIDGET_FANCYUI_SLIDERSTYLE_H
