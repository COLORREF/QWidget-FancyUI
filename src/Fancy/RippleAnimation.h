//
// Created by TX on 2025/10/3.
//

#ifndef QWIDGET_FANCYUI_RIPPLEANIMATION_H
#define QWIDGET_FANCYUI_RIPPLEANIMATION_H

#include <QObject>
#include <QQueue>
#include <QVariantAnimation>

#include "ParallelAnimationGroupPool.h"

class QPainter;

namespace fancy
{
    class ParallelAnimationGroupPool;

    class VariantAnimation : public QVariantAnimation
    {
        Q_OBJECT

    public:
        explicit VariantAnimation(QObject *parent = nullptr);

        const QVariant &runTimeValue() { return _runtimeValue; }

    public slots:
        void setRuntimeValue(const QVariant &runTime) { _runtimeValue = runTime; }

    private:
        QVariant _runtimeValue;
    };

    class RippleAnimation : public QObject
    {
        Q_OBJECT

    public:
        RippleAnimation(int msecs, int maxRadius, const QColor &startColor, const QColor &endColor, quint32 initialQuantity, QObject *parent);

        void setUpdate(QWidget *device);

        void paint(QPainter *painter) const;

        int animationQuantity() const { return _pool->idleSize() + _pool->runningSize(); }

    public slots:
        void updateMaxRadius(int maxRadius);

        void updateStartColor(const QColor &startColor);

        void updateEndColor(const QColor &endColor);

        void start(QPointF pt);

    private:
        ParallelAnimationGroupPool *_pool;
        QQueue<QPointF> _paintPoint; // 绘制坐标
    };
} // fancy

#endif //QWIDGET_FANCYUI_RIPPLEANIMATION_H
