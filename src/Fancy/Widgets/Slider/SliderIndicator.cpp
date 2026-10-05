//
// Created by TX on 2025/9/26.
//

#include "SliderIndicator.h"

#include <QMouseEvent>
#include <QPainter>
#include <QVariantAnimation>
#include "Core/Defs.h"
#include "Core/Palette/Palette.h"

namespace fancy
{
    namespace
    {
        constexpr qreal borderSpace = 1.0;
        constexpr qreal normalRatio = 10.0 / 20.0;
        constexpr qreal activeRatio = 14.0 / 20.0;
        constexpr qreal pressedRatio = 8.0 / 20.0;
    }

    SliderIndicator::SliderIndicator(QWidget *parent) :
        QWidget(parent),
        _ani(new QVariantAnimation(this))
    {
        QRectF rect_ = rect();
        const qreal inset = borderSpace / 2;
        rect_.adjust(inset, inset, -inset, -inset);
        _r = qMin(rect_.width(), rect_.height()) / 2;
        _center = rect_.center();
        _aniValue = normalRatio;

        _ani->setStartValue(0.0f);
        _ani->setEndValue(0.0f);
        _ani->setDuration(100);

        connect(_ani,
                &QVariantAnimation::valueChanged,
                this,
                [this](const QVariant &value) {
                    _aniValue = value.toReal();
                    update();
                }
        );
    }

    void SliderIndicator::animateState()
    {
        _ani->stop();
        _ani->setStartValue(_aniValue);
        _ani->setEndValue(_pressed ? pressedRatio : (_isContains ? activeRatio : normalRatio));
        _ani->start();
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void SliderIndicator::enterEvent(QEnterEvent *event)
#else
    void SliderIndicator::enterEvent(QEvent *event)
#endif
    {
        _isContains = true;
        animateState();
        QWidget::enterEvent(event);
    }

    void SliderIndicator::leaveEvent(QEvent *event)
    {
        _isContains = false;
        animateState();
        QWidget::leaveEvent(event);
    }

    void SliderIndicator::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            _pressed = true;
            animateState();
        }
        QWidget::mousePressEvent(event);
    }

    void SliderIndicator::mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            _pressed = false;
            animateState();
        }
        // 不在里面直接触发leaveEvent
        return QWidget::mouseReleaseEvent(event);
    }

    void SliderIndicator::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        QRectF rect_ = rect();
        const qreal inset = borderSpace / 2;
        rect_.adjust(inset, inset, -inset, -inset);
        _r = qMax(0.0, qMin(rect_.width(), rect_.height()) / 2);
        _center = rect_.center();
    }

    void SliderIndicator::paintEvent(QPaintEvent *paint_event)
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing);
        auto &palette = Palette::palette();
        painter.setPen(Qt::NoPen);
        painter.setBrush(palette[ColorRole::Button]);
        painter.drawEllipse(_center, _r, _r);
        painter.setPen(Qt::NoPen);
        painter.setBrush(palette[ColorRole::AppAccent]);
        painter.drawEllipse(_center, _r * _aniValue, _r * _aniValue);
    }
} // fancy
