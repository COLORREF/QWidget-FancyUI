//
// Created by A1840 on 2026/8/31.
//

#ifndef QWIDGET_FANCYUI_UTILS_H
#define QWIDGET_FANCYUI_UTILS_H
#include <QImage>
#include <QWidget>
#include <QRect>
#include <QLine>
#include <QtMath>

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

    // region 使用逻辑坐标；resolutionScale 为原生分辨率的采样比例，1 为原生，0.5 为宽高各减半
    // 无效区域或非正、非有限的比例返回空图像；返回图像的 DPR 包含采样比例
    inline QImage renderWidgetRegion(QWidget *widget, const QRect &region, const qreal resolutionScale = 1.0)
    {
        if (!widget || region.isEmpty() || !qIsFinite(resolutionScale) || resolutionScale <= 0)
            return {};
        const qreal dpr = widget->devicePixelRatioF() * resolutionScale;
        // 向下取整可避免图像的逻辑范围超过 region，防止多出的透明边缘被模糊到图像内部
        const QSize pixelSize(qMax(1, qFloor(region.width() * dpr)), qMax(1, qFloor(region.height() * dpr)));
        QImage image(pixelSize, QImage::Format_ARGB32_Premultiplied);
        if (image.isNull())
            return {};
        image.setDevicePixelRatio(dpr);
        image.fill(Qt::GlobalColor::transparent);
        if (const QRect intersected = region.intersected({{0, 0}, widget->size()}); !intersected.isEmpty())
            widget->render(
                &image,
                intersected.topLeft() - region.topLeft(),
                intersected,
                QWidget::RenderFlag::DrawWindowBackground);
        return image;
    }

    inline QRect getWidgetRectInAncestor(const QWidget *child, const QWidget *ancestor)
    {
        if (!child || !ancestor)
            return {};
        return {child->mapTo(ancestor, QPoint{0, 0}), child->size()};
    }
}

#endif //QWIDGET_FANCYUI_UTILS_H
