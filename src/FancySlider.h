#ifndef FANCYSLIDER_H
#define FANCYSLIDER_H

#include <QObject>
#include <QSlider>
#include <QWidget>
#include "FancyStyle.h"

class FancySlider : public QSlider
{
    Q_OBJECT
public:
    explicit FancySlider(QWidget *parent = nullptr):
        QSlider(parent)
    {
        this->setStyle(new SliderStyle(this));
        this->style()->setParent(this);
    }
    explicit FancySlider(Qt::Orientation orientation, QWidget *parent = nullptr):
        QSlider(orientation,parent)
    {
        this->setStyle(new SliderStyle(this,orientation));
        this->style()->setParent(this);
    }

    void setOrientation(Qt::Orientation orientation)
    {
        QSlider::setOrientation(orientation);
        dynamic_cast<SliderStyle*>(this->style())->setOrientation(orientation);
    }
};

#endif // FANCYSLIDER_H
