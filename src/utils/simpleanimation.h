#ifndef SIMPLEANIMATION_H
#define SIMPLEANIMATION_H

#include <QVariantAnimation>
#include <QWidget>

class SimpleAnimation : public QVariantAnimation
{
public:
    SimpleAnimation(const QVariant &start, const QVariant &end, int time_msecs, bool isforward, QObject *parent = nullptr)
        : QVariantAnimation(parent)
    {
        this->setStartValue(start);
        this->setEndValue(end);
        this->setDuration(time_msecs);
        this->setDirection(isforward);
    }

    using QVariantAnimation::setDirection;
    inline void setUpdate(QWidget *device)
    {
        connect(this, &QVariantAnimation::valueChanged, device, QOverload<>::of(&QWidget::update));
        // connect(this,&QVariantAnimation::valueChanged,device,static_cast<void(QWidget::*)()>(&QWidget::update));
    };
public slots:
    inline void setDirection(bool isforward = true) { QVariantAnimation::setDirection((Direction)!isforward); };
    inline void setForward() { setDirection(Direction::Forward); }
    inline void setBackward() { setDirection(Direction::Backward); }
    inline void reverseDirectionAndStartWithPolicy(QAbstractAnimation::DeletionPolicy policy = KeepWhenStopped)
    {
        reverseDirection();
        start(policy);
    };
    inline void reverseDirectionAndStart()
    {
        reverseDirection();
        start();
    };
    inline void reverseDirection() { QVariantAnimation::setDirection((Direction)!direction()); }
};

#endif // SIMPLEANIMATION_H
