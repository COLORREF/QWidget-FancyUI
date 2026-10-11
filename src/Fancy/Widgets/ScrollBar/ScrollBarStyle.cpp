//
// Created by TX on 2025/10/16.
//

#include "ScrollBarStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QStyleOption>
#include "Core/Defs.h"
#include "Core/ControlState/ControlState.h"
#include "Core/Palette/Palette.h"

namespace fancy
{
    namespace
    {
        constexpr int scrollBarExtent = 12;
        constexpr qreal normalThumbWidth = 2;
        constexpr qreal activeThumbWidth = 6;
        constexpr qreal arrowWidth = 8;
        constexpr int arrowLength = 6;
        constexpr int arrowEndMargin = 5;
        constexpr int arrowThumbGap = 4;
        constexpr int endExtent = arrowEndMargin + arrowLength + arrowThumbGap;
    }

    ScrollBarStyle::ScrollBarStyle(QScrollBar *parent) :
        QProxyStyle(nullptr)
    {
        setParent(parent);
        parent->setAttribute(Qt::WidgetAttribute::WA_OpaquePaintEvent, false);
    }

    void ScrollBarStyle::drawComplexControl(
        ComplexControl control,
        const QStyleOptionComplex *option,
        QPainter *painter,
        const QWidget *widget) const
    {
        if (const auto *opt = qstyleoption_cast<const QStyleOptionSlider *>(option); control == CC_ScrollBar && opt)
        {
            auto &pal = Palette::palette();
            const ControlState state(opt->state, ControlType::Widget);
            const bool isHorizontal = opt->orientation == Qt::Horizontal;
            const bool isActive = state == VisualState::Hover || state == VisualState::Pressed || (state == VisualState::Disabled && state._over);
            const QRectF sliderRect = subControlRect(control, option, SC_ScrollBarSlider, widget);
            const QRectF rect = opt->rect;
            const qreal center = isHorizontal ? rect.center().y() : rect.center().x(); // 中心坐标
            const qreal bgRadius = (isHorizontal ? opt->rect.height() : opt->rect.width()) / 2.0; // 背景圆角
            const QRectF addArrowRect = isHorizontal
                                            ? QRectF(rect.right() - arrowEndMargin - arrowLength, center - arrowWidth / 2, arrowLength, arrowWidth)
                                            : QRectF(center - arrowWidth / 2, rect.bottom() - arrowEndMargin - arrowLength, arrowWidth, arrowLength);
            const QRectF subArrowRect = isHorizontal
                                            ? QRectF(rect.left() + arrowEndMargin, center - arrowWidth / 2, arrowLength, arrowWidth)
                                            : QRectF(center - arrowWidth / 2, rect.top() + arrowEndMargin, arrowWidth, arrowLength);
            const qreal thumbWidth = isActive ? activeThumbWidth : normalThumbWidth;
            // 收起时细条靠右（水平滚动条靠下），距外侧边缘 2 px。
            const qreal thumbCenter = isActive
                                          ? center
                                          : (isHorizontal ? rect.bottom() : rect.right()) - 2 - thumbWidth / 2;
            const QRectF slider = isHorizontal
                                      ? QRectF(sliderRect.left(), thumbCenter - thumbWidth / 2, sliderRect.width(), thumbWidth)
                                      : QRectF(thumbCenter - thumbWidth / 2, sliderRect.top(), thumbWidth, sliderRect.height());
            const qreal sdRadius = (isHorizontal ? slider.height() : slider.width()) / 2.0; // 滑块圆角

            const auto drawArrow = [painter, &addArrowRect, &subArrowRect](const bool horizontal) {
                if (horizontal)
                {
                    ScrollBarStyle::drawArrow(painter, addArrowRect, Direction::Right);
                    ScrollBarStyle::drawArrow(painter, subArrowRect, Direction::Left);
                }
                else
                {
                    ScrollBarStyle::drawArrow(painter, addArrowRect, Direction::Down);
                    ScrollBarStyle::drawArrow(painter, subArrowRect, Direction::Up);
                }
            };

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->setPen(Qt::NoPen);

            // 绘制背景
            painter->setBrush(pal[isActive ? ColorRole::ScrollBarFocus : ColorRole::ScrollBarNormal]);
            painter->drawRoundedRect(opt->rect, bgRadius, bgRadius);

            // 绘制滑块
            painter->setBrush(pal[ColorRole::ScrollBarSlider]);
            painter->drawRoundedRect(slider, sdRadius, sdRadius);

            // 绘制箭头
            if (isActive && (isHorizontal ? rect.width() : rect.height()) >= 2 * endExtent)
                drawArrow(isHorizontal);
            painter->restore();
            return;
        }

        QProxyStyle::drawComplexControl(control, option, painter, widget);
    }


    int ScrollBarStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        switch (metric)
        {
            case PM_ScrollBarExtent: // 12 垂直滚动条的宽度和水平滚动条的高度
                return scrollBarExtent;
            case PM_ScrollBarSliderMin: // 18 垂直滚动条滑块的最小高度和水平滚动条滑块的最小宽度
                return 18;
            default:
                break;
        }
        return QProxyStyle::pixelMetric(metric, option, widget);
    }

    QRect ScrollBarStyle::subControlRect(ComplexControl control, const QStyleOptionComplex *option, SubControl subControl, const QWidget *widget) const
    {
        const auto *opt = qstyleoption_cast<const QStyleOptionSlider *>(option);
        if (control != CC_ScrollBar || !opt)
            return QProxyStyle::subControlRect(control, option, subControl, widget);

        const bool horizontal = opt->orientation == Qt::Horizontal;
        const QRect rect = opt->rect;
        const int length = qMax(0, horizontal ? rect.width() : rect.height());
        const int buttonLength = qMin(endExtent, length / 2);
        const int grooveLength = length - 2 * buttonLength;
        const qint64 range = qMax(qint64(0), qint64(opt->maximum) - qint64(opt->minimum));
        const qint64 pageStep = qMax(0, opt->pageStep);
        const int thumbLength =
                range == 0
                    ? grooveLength
                    : qMin(grooveLength, qMax(pixelMetric(PM_ScrollBarSliderMin, option, widget), static_cast<int>(pageStep * grooveLength / (range + pageStep))));
        const int thumbStart =
                buttonLength +
                sliderPositionFromValue(opt->minimum, opt->maximum, opt->sliderPosition, grooveLength - thumbLength, opt->upsideDown);

        // 绘制、点击和拖拽共用同一套端部留白，滑块到端点时仍留 4 px 空隙。
        const auto section = [&](int start, int span) {
            const QRect result = horizontal
                                     ? QRect(rect.x() + start, rect.y(), span, rect.height())
                                     : QRect(rect.x(), rect.y() + start, rect.width(), span);
            return horizontal ? visualRect(opt->direction, rect, result) : result;
        };
        switch (subControl)
        {
            case SC_ScrollBarSubLine:
                return section(0, buttonLength);
            case SC_ScrollBarAddLine:
                return section(length - buttonLength, buttonLength);
            case SC_ScrollBarGroove:
                return section(buttonLength, grooveLength);
            case SC_ScrollBarSlider:
                return section(thumbStart, thumbLength);
            case SC_ScrollBarSubPage:
                return section(buttonLength, thumbStart - buttonLength);
            case SC_ScrollBarAddPage:
                return section(thumbStart + thumbLength,
                               length - buttonLength - thumbStart - thumbLength);
            default:
                return QProxyStyle::subControlRect(control, option, subControl, widget);
        }
    }

    void ScrollBarStyle::drawArrow(QPainter *painter, const QRectF &arrowRect, const Direction dir)
    {
        // 8 × 6 的圆角实心三角形；只填充，避免描边向外扩张尺寸。
        QPainterPath path;
        path.moveTo(1, 0);
        path.lineTo(7, 0);
        path.quadTo(8, 0, 8, 1);
        path.quadTo(8, 1.3, 7.8, 1.6);
        path.lineTo(4.8, 5.6);
        path.quadTo(4, 6.4, 3.2, 5.6);
        path.lineTo(0.2, 1.6);
        path.quadTo(0, 1.3, 0, 1);
        path.quadTo(0, 0, 1, 0);
        path.closeSubpath();

        QTransform transform;
        transform.translate(arrowRect.center().x(), arrowRect.center().y());
        switch (dir)
        {
            case Direction::Right:
                transform.rotate(-90);
                break;
            case Direction::Left:
                transform.rotate(90);
                break;
            case Direction::Up:
                transform.rotate(180);
                break;
            case Direction::Down:
                break;
            default:
                return;
        }
        transform.translate(-arrowWidth / 2, -arrowLength / 2.0);
        painter->fillPath(transform.map(path), Palette::palette()[ColorRole::ScrollBarSlider]);
    }
} // fancy
