//
// Created by TX on 2025/9/26.
//

#include "Slider.h"

#include "SliderStyle.h"

namespace fancy
{
    Slider::Slider(QWidget *parent) :
        QSlider(parent)
    {
        setStyle(new SliderStyle(this));
    }

    Slider::Slider(Qt::Orientation orientation, QWidget *parent) :
        QSlider(orientation, parent)
    {
        setStyle(new SliderStyle(this, orientation));
    }

    void Slider::setOrientation(Qt::Orientation orientation)
    {
        QSlider::setOrientation(orientation);
        if (const auto style_ = dynamic_cast<SliderStyle *>(style()))
            style_->setOrientation(orientation);
    }
} // fancy
