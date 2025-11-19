//
// Created by TX on 2025/10/15.
//

#include "CenteredImageWidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

#include "Defs.h"
#include "Palette.h"

namespace fancy
{
    CenteredImageWidget::CenteredImageWidget(QWidget *parent, int radius) :
        QWidget(parent),
        _radius(radius),
        _drawBorder(false),
        _drawMask(false),
        _fillBackground(false),
        _mask(Qt::GlobalColor::transparent),
        _bkFill(Qt::GlobalColor::transparent)
    {
        setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        _debounceTimer.setSingleShot(true);
        _debounceTimer.setInterval(100);
        connect(&_debounceTimer, &QTimer::timeout, this, &CenteredImageWidget::updateSmoothScaledPixmap);
    }

    CenteredImageWidget::CenteredImageWidget(QWidget *parent, const QPixmap &pixmap, int radius) :
        CenteredImageWidget(parent, radius)
    {
        setPixmap(pixmap);
    }

    void CenteredImageWidget::setPixmap(const QPixmap &pixmap)
    {
        _pixmap = pixmap;
        updateFastScaledPixmap();
        _debounceTimer.start();
    }

    void CenteredImageWidget::setRadius(int radius)
    {
        _radius = radius;
        updateClipPath();
        update();
    }

    void CenteredImageWidget::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);

        updateClipPath();
        updateFastScaledPixmap();
        _debounceTimer.start(); // 防抖定时器
    }

    void CenteredImageWidget::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::SmoothPixmapTransform);
        painter.setClipPath(_painterPath);
        if (_fillBackground)
            painter.fillPath(_painterPath, _bkFill);
        // 优先绘制平滑图，否则绘制快速图
        if (!_scaledSmooth.isNull())
            painter.drawPixmap(_offset, _scaledSmooth);
        else if (!_scaledFast.isNull())
            painter.drawPixmap(_offset, _scaledFast);
        if (_drawMask)
            painter.fillPath(_painterPath, _mask);
        if (_drawBorder)
            painter.strokePath(_painterPath, QPen(Palette::palette()[ColorRole::Border], 1));
    }

    void CenteredImageWidget::updateClipPath()
    {
        _painterPath.clear();
        _painterPath.addRoundedRect(rect(), _radius, _radius);
    }

    void CenteredImageWidget::updateFastScaledPixmap()
    {
        if (_pixmap.isNull() || size().isEmpty())
            return;
        _scaledFast = _pixmap.scaled(size(), Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::TransformationMode::FastTransformation);
        _offset = QPoint((width() - _scaledFast.width()) / 2, (height() - _scaledFast.height()) / 2);
        _scaledSmooth = QPixmap(); // 清空旧的平滑图（保证先显示快速版本）
        update();
        emit FastTPixmapUpdate();
    }

    void CenteredImageWidget::updateSmoothScaledPixmap()
    {
        if (_pixmap.isNull() || size().isEmpty())
            return;
        _scaledSmooth = _pixmap.scaled(size(), Qt::AspectRatioMode::KeepAspectRatioByExpanding, Qt::TransformationMode::SmoothTransformation);
        _offset = QPoint((width() - _scaledSmooth.width()) / 2, (height() - _scaledSmooth.height()) / 2);
        update();
        emit SmoothPixmapUpdate();
    }
} // fancy
