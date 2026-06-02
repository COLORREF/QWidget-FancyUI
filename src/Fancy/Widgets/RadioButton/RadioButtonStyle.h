//
// Created by TX on 2025/8/30.
//

#ifndef QWIDGET_FANCYUI_RADIOBUTTONSTYLE_H
#define QWIDGET_FANCYUI_RADIOBUTTONSTYLE_H

#include <QProxyStyle>
#include <QRadioButton>

namespace fancy
{
    class RadioButtonStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit RadioButtonStyle(QRadioButton *parent);

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override;

    private slots:
        void setInnerRadius(const QVariant &value);

    private:
        mutable bool _un_enable_on = false;
        qreal _inner_radius = 0;
    };
} // fancy

#endif //QWIDGET_FANCYUI_RADIOBUTTONSTYLE_H
