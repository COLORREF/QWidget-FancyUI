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

        int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override;

        QRect subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl,
                             const QWidget *widget = nullptr) const override;

    protected:
        void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const override;

    private:
        SliderIndicator *_indicator = nullptr;
    };
} // fancy

#endif //QWIDGET_FANCYUI_SLIDERSTYLE_H
