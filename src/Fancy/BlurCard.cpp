//
// Created by TX on 2025/10/30.
//

#include "BlurCard.h"

#include <QPainterPath>

#include "Defs.h"

namespace fancy
{
    BlurCard::BlurCard(QWidget *parent) :
        BlurCard(parent, nullptr) {}

    BlurCard::BlurCard(QWidget *parent, QWidget *blurredObj) :
        QPushButton(parent),
        _blurRadius(10),
        _radius(10),
        _blurredObj(blurredObj) {}

    void BlurCard::blur()
    {
        if (_blurredObj)
            setPixmap(QPixmap::fromImage(gaussianBlur(renderWidgetRegion(_blurredObj, getWidgetRectInAncestor(this, _blurredObj)), _blurRadius)));
    }

    void BlurCard::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::SmoothPixmapTransform);
        QPainterPath path;
        path.addRoundedRect(rect(), _radius, _radius);
        painter.setClipPath(path);
        if (!_pixmap.isNull())
            painter.drawPixmap(rect(), _pixmap);
    }
} // fancy
