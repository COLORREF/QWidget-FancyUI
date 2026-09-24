//
// Created by A1840 on 2026/8/31.
//

#ifndef QWIDGET_FANCYUI_UTILS_H
#define QWIDGET_FANCYUI_UTILS_H
#include <QImage>
#include <QWidget>
#include <QRect>
#include <QLine>

namespace fancy
{
    inline QLine bottomLine(const QRect &rect, const int filletRadius)
    {
        if (int x = rect.center().x(); rect.width() - 2 * filletRadius <= 0)
            return {x, rect.y() + rect.height(), x, rect.y() + rect.height()};
        return {
            rect.x() + filletRadius,
            rect.y() + rect.height(),
            rect.x() + rect.width() - filletRadius,
            rect.y() + rect.height()
        };
    }

    inline QImage renderWidgetRegion(QWidget *widget, const QRect &region)
    {
        if (!widget)
            return {};
        // const qreal dpr = widget->devicePixelRatioF();
        QImage image(region.size() /* *dpr */, QImage::Format_ARGB32_Premultiplied);
        // image.setDevicePixelRatio(dpr);
        image.fill(Qt::GlobalColor::black);
        if (const QRect intersected = region.intersected({{0, 0}, widget->size()}); !intersected.isEmpty())
            widget->render(&image, QPoint(0, 0), intersected, QWidget::RenderFlag::DrawWindowBackground);
        return image;
    }

    inline QRect getWidgetRectInAncestor(const QWidget *child, const QWidget *ancestor)
    {
        if (!child || !ancestor)
            return {};
        return {child->mapTo(ancestor, QPoint{0, 0}), child->mapTo(ancestor, QPoint{child->width(), child->height()})};
    }

}

#endif //QWIDGET_FANCYUI_UTILS_H
