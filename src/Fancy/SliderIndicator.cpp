//
// Created by TX on 2025/9/26.
//

#include "SliderIndicator.h"

#include <QMouseEvent>
#include <QPainter>
#include <QVariantAnimation>

#include "Defs.h"
#include "Palette.h"

namespace fancy
{
    SliderIndicator::SliderIndicator(QWidget *parent) :
        QWidget(parent),
        _ani(new QVariantAnimation(this))
    {
        QRectF rect_ = rect();
        rect_.adjust(0.5, 0.5, -0.5, -0.5);
        _r = qMin(rect_.width(), rect_.height()) / 2;
        _aniValue = _r / 2;

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

    void SliderIndicator::enterEvent(QEnterEvent *event)
    {
        _isContains = true;
        _ani->setStartValue(_r / 2);
        _ani->setEndValue(_r / 1.3);
        _ani->start();
        QWidget::enterEvent(event);
    }

    void SliderIndicator::leaveEvent(QEvent *event)
    {
        _isContains = false;
        _ani->setStartValue(_aniValue);
        _ani->setEndValue(_r / 2);
        _ani->start();
        QWidget::leaveEvent(event);
    }

    void SliderIndicator::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
        {
            _ani->setStartValue(_aniValue);
            _ani->setEndValue(_r / 2.8);
            _ani->start();
        }
        QWidget::mousePressEvent(event);
    }

    void SliderIndicator::mouseReleaseEvent(QMouseEvent *event)
    {
        if (_isContains && event->button() == Qt::LeftButton)
        {
            _ani->setStartValue(_r / 2.5);
            _ani->setEndValue(_r / 1.3);
            _ani->start();
        }
        // 不在里面直接触发leaveEvent
        return QWidget::mouseReleaseEvent(event);
    }

    void SliderIndicator::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        QRectF rect_ = rect();
        rect_.adjust(0.5, 0.5, -0.5, -0.5);
        _r = qMin(rect_.width(), rect_.height()) / 2;
        _center = rect_.center();
        _aniValue = _r / 2;
    }

    void SliderIndicator::paintEvent(QPaintEvent *paint_event)
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing);
        auto &palette = Palette::palette();
        painter.setPen(QPen(palette[ColorRole::Border], 1));
        painter.setBrush(palette[ColorRole::Button]);
        painter.drawEllipse(_center, _r, _r);
        painter.setPen(Qt::NoPen);
        painter.setBrush(palette[ColorRole::AppAccent]);
        painter.drawEllipse(_center, _aniValue, _aniValue);
    }
} // fancy
