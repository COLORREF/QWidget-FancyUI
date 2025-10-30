//
// Created by TX on 2025/10/26.
//

#ifndef QWIDGET_FANCYUI_ANIMATOR_H
#define QWIDGET_FANCYUI_ANIMATOR_H
#include <QAbstractAnimation>
#include <QEasingCurve>
#include <QVariant>

class QParallelAnimationGroup;
class QPropertyAnimation;
class QVariantAnimation;

namespace fancy
{
    struct AnimationTask
    {
        QObject *parent;
        QObject *target;
        QByteArray property;
        QVariant startValue;
        QVariant endValue;
        int duration;
        QEasingCurve easing = QEasingCurve::Linear;
        QAbstractAnimation::Direction direction = QAbstractAnimation::Forward;
    };

    class Animator : public QObject
    {
        Q_OBJECT

    public:
        explicit Animator(QObject *parent = nullptr);

        static QPropertyAnimation *makeProperty(const AnimationTask &t);

        static QVariantAnimation *makeVariant(const AnimationTask &t);

        static QParallelAnimationGroup *makeParallel(const QList<QAbstractAnimation *> &anis, QObject *parent = nullptr);

        static QSequentialAnimationGroup *makeSequential(const QList<QAbstractAnimation *> &anis, QObject *parent = nullptr);

        static void reverse(QAbstractAnimation *ani);
    };
} // fancy

#endif //QWIDGET_FANCYUI_ANIMATOR_H
