//
// Created by TX on 2025/10/26.
//

#include "Animator.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QVariantAnimation>

#include "ParallelAnimationGroupPool.h"

namespace fancy
{
    Animator::Animator(QObject *parent) :
        QObject(parent) {}

    QPropertyAnimation *Animator::makeProperty(const AnimationTask &t)
    {
        if (!t.target)
            return nullptr;
        auto *ani = new QPropertyAnimation(t.target, t.property, t.parent ? t.parent : t.target);
        ani->setStartValue(t.startValue);
        ani->setEndValue(t.endValue);
        ani->setDuration(t.duration);
        ani->setEasingCurve(t.easing);
        ani->setDirection(t.direction);
        return ani;
    }

    QVariantAnimation *Animator::makeVariant(const AnimationTask &t)
    {
        auto *ani = new QVariantAnimation(t.parent);
        ani->setStartValue(t.startValue);
        ani->setEndValue(t.endValue);
        ani->setDuration(t.duration);
        ani->setEasingCurve(t.easing);
        ani->setDirection(t.direction);
        return ani;
    }

    QParallelAnimationGroup *Animator::makeParallel(const QList<QAbstractAnimation *> &anis, QObject *parent)
    {
        auto *group = new QParallelAnimationGroup(parent);
        for (auto *ani: anis)
            if (ani)
                group->addAnimation(ani);
        return group;
    }

    QSequentialAnimationGroup *Animator::makeSequential(const QList<QAbstractAnimation *> &anis, QObject *parent)
    {
        auto *group = new QSequentialAnimationGroup(parent);
        for (auto *ani: anis)
            if (ani)
                group->addAnimation(ani);
        return group;
    }

    void Animator::reverse(QAbstractAnimation *ani)
    {
        if (!ani)
            return;
        ani->setDirection(ani->direction() == QAbstractAnimation::Forward ? QAbstractAnimation::Backward : QAbstractAnimation::Forward);
    }
} // fancy
