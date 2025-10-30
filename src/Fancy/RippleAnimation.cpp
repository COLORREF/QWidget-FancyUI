//
// Created by TX on 2025/10/3.
//

#include "RippleAnimation.h"

#include <QPainter>
#include <QPointF>
#include <QVariant>
#include <QVariantAnimation>
#include <QWidget>

#include "magic_enum.hpp"
#include "ParallelAnimationGroupPool.h"

namespace fancy
{
    VariantAnimation::VariantAnimation(QObject *parent) :
        QVariantAnimation(parent)
    {
        connect(this, &QVariantAnimation::valueChanged, this, &VariantAnimation::setRuntimeValue);
    }

    RippleAnimation::RippleAnimation(int msecs, int maxRadius, const QColor &startColor, const QColor &endColor, quint32 initialQuantity, QObject *parent) :
        QObject(parent)
    {
        auto factory = [=]()-> QParallelAnimationGroup * {
            QParallelAnimationGroup *group = new QParallelAnimationGroup(parent);

            VariantAnimation *r_ani = new VariantAnimation(group);
            r_ani->setDuration(msecs);
            r_ani->setStartValue(0);
            r_ani->setEndValue(maxRadius);
            r_ani->setDirection(QVariantAnimation::Direction::Backward);
            r_ani->setRuntimeValue(0);

            VariantAnimation *h_ani = new VariantAnimation(group);
            h_ani->setDuration(msecs);
            h_ani->setStartValue(startColor);
            h_ani->setEndValue(endColor);
            r_ani->setDirection(QVariantAnimation::Direction::Backward);
            h_ani->setRuntimeValue(QColor(Qt::GlobalColor::transparent));

            group->addAnimation(r_ani);
            group->addAnimation(h_ani);
            connect(group, &QParallelAnimationGroup::finished, this, [this]() { _paintPoint.dequeue(); });
            return group;
        };

        _pool = new ParallelAnimationGroupPool(factory, this);
        for (quint32 i = 0; i < initialQuantity; i++)
            _pool->addGroup(factory());
    }

    void RippleAnimation::setUpdate(QWidget *device)
    {
        for (const QParallelAnimationGroup *group: _pool->idle())
        {
            connect(static_cast<QVariantAnimation *>(group->animationAt(0)), &QVariantAnimation::valueChanged, device, QOverload<>::of(&QWidget::update));
            connect(static_cast<QVariantAnimation *>(group->animationAt(1)), &QVariantAnimation::valueChanged, device, QOverload<>::of(&QWidget::update));
        }
    }

    void RippleAnimation::paint(QPainter *painter) const
    {
        painter->save();
        painter->setPen(Qt::PenStyle::NoPen);
        const QQueue<QParallelAnimationGroup *> &groups = _pool->running();
        for (QParallelAnimationGroup *group: groups)
        {
            painter->setBrush(static_cast<VariantAnimation *>(group->animationAt(1))->runTimeValue().value<QColor>());
            int r = static_cast<VariantAnimation *>(group->animationAt(0))->runTimeValue().toInt();
            painter->drawEllipse(_paintPoint.at(groups.indexOf(group)), r, r);
        }
        painter->restore();
    }

    void RippleAnimation::updateMaxRadius(int maxRadius)
    {
        for (const QParallelAnimationGroup *group: _pool->idle())
            static_cast<VariantAnimation *>(group->animationAt(0))->setEndValue(maxRadius);
    }

    void RippleAnimation::updateStartColor(const QColor &startColor)
    {
        for (const QParallelAnimationGroup *group: _pool->idle())
            static_cast<VariantAnimation *>(group->animationAt(1))->setStartValue(startColor);
    }

    void RippleAnimation::updateEndColor(const QColor &endColor)
    {
        for (const QParallelAnimationGroup *group: _pool->idle())
            static_cast<VariantAnimation *>(group->animationAt(1))->setEndValue(endColor);
    }

    void RippleAnimation::start(QPointF pt)
    {
        _paintPoint.enqueue(pt);
        _pool->acquireGroupAndStart();
    }
} // fancy
