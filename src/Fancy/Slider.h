//
// Created by TX on 2025/9/26.
//

#ifndef QWIDGET_FANCYUI_SLIDER_H
#define QWIDGET_FANCYUI_SLIDER_H
#include <QSlider>
#include <QWidget>

namespace fancy
{
    class Slider : public QSlider
    {
        Q_OBJECT

    public:
        explicit Slider(QWidget *parent = nullptr);

        explicit Slider(Qt::Orientation orientation, QWidget *parent = nullptr);

        // NOLINTNEXTLINE
        void setOrientation(Qt::Orientation);
    };
} // fancy

#endif //QWIDGET_FANCYUI_SLIDER_H
