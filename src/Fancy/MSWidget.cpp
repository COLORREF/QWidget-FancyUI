//
// Created by TX on 2025/10/22.
//

#include "MSWidget.h"

#include <QPainterPath>
#include <QVariantAnimation>

#include "Defs.h"
#include "Palette.h"
#include "ThemeModeController.h"

namespace fancy
{
    MSWidget::MSWidget(QWidget *parent) :
        QWidget(parent),
        _animation(new QVariantAnimation(this)),
        _color(Palette::palette()[ColorRole::MSWindow])
    {
        _animation->setStartValue(Palette::palette().color(ColorRole::MSWindow, Theme::Light));
        _animation->setEndValue(Palette::palette().color(ColorRole::MSWindow, Theme::Dark));
        _animation->setDuration(300);
        connect(_animation, &QVariantAnimation::valueChanged, this, &MSWidget::updateBkgColor);
        connect(&Palette::palette(), &Palette::appThemeChange, this, &MSWidget::onThemeChanged);
    }

    void MSWidget::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.translate(0.5, 0.5);
        QPainterPath path;
        path.addRoundedRect(1, 1, width() + 10, height() + 10, 5, 5);
        painter.fillPath(path, _color); // 填充
        painter.strokePath(path, {Palette::palette()[ColorRole::MSWindowSplitLine], 1}); // 描边
    }

    void MSWidget::onThemeChanged()
    {
        if (const auto theme = ThemeModeController::controller().appTheme(); theme == Theme::Light)
            _animation->setDirection(QAbstractAnimation::Direction::Backward);
        else
            _animation->setDirection(QAbstractAnimation::Direction::Forward);
        _animation->start();
    }

    void MSWidget::updateBkgColor(const QVariant &value)
    {
        _color = value.value<QColor>();
        update();
    }
} // fancy
