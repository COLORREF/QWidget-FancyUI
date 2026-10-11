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
#include <QFontMetrics>
#include <QStringList>

namespace fancy
{
    // 按显式换行拆分文本，统一换行格式并保留空行和末尾换行
    inline QStringList textLines(QString text)
    {
        text.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));
        text.replace(QChar('\r'), QChar('\n'));
        return text.split(QChar('\n'), Qt::KeepEmptyParts);
    }

    // 测量显式多行文本，不自动折行；空文本返回空尺寸
    inline QSize textSize(const QFontMetrics &metrics, const QString &text)
    {
        if (text.isEmpty())
            return {};
        const auto lines = textLines(text);
        int width = 0;
        for (const auto &line: lines)
            width = qMax(width, metrics.horizontalAdvance(line));
        return {width, metrics.height() + (static_cast<int>(lines.size()) - 1) * metrics.lineSpacing()};
    }

    // 将图标可见尺寸按指定比例换算为布局尺寸，比例由调用方的样式决定
    inline QSize iconLayoutSize(const QSize &visibleSize, qreal layoutScale)
    {
        return {qRound(visibleSize.width() * layoutScale), qRound(visibleSize.height() * layoutScale)};
    }

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
