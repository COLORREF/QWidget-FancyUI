//
// Created by TX on 2025/10/16.
//

#include "ScrollBarStyle.h"

#include <QScrollBar>

#include "CheckBoxStyle.h"
#include "ControlState.h"
#include "Defs.h"
#include "Palette.h"

namespace fancy
{
    ScrollBarStyle::ScrollBarStyle(QScrollBar *parent) :
        QProxyStyle(nullptr)
    {
        setParent(parent);
        parent->setAttribute(Qt::WidgetAttribute::WA_OpaquePaintEvent, false);
    }

    void ScrollBarStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
    {
        if (const auto *opt = qstyleoption_cast<const QStyleOptionSlider *>(option); control == CC_ScrollBar && opt)
        {
            auto &pal = Palette::palette();
            const ControlState state(opt->state, ControlType::Widget);
            const bool isHorizontal = opt->orientation == Qt::Horizontal;
            const bool isActive = state == VisualState::Hover || state == VisualState::Pressed || (state == VisualState::Disabled && state._over);
            const QRectF sliderRect = subControlRect(control, option, SC_ScrollBarSlider, widget);
            const QRectF grooveRect = subControlRect(control, option, SC_ScrollBarGroove, widget);
            const QRectF rect = opt->rect;
            const qreal center = isHorizontal ? sliderRect.center().y() : sliderRect.center().x(); // 中心坐标
            const qreal bgRadius = (isHorizontal ? opt->rect.height() : opt->rect.width()) / 2.0; // 背景圆角
            const QRectF addArrowRect = isHorizontal
                                            ? QRectF{grooveRect.topRight(), rect.bottomRight()}.adjusted(4, 2, -1, -2)
                                            : QRectF{grooveRect.bottomLeft(), rect.bottomRight()}.adjusted(2, 4, -2, -1);
            const QRectF subArrowRect = isHorizontal
                                            ? QRectF{rect.topLeft(), grooveRect.bottomLeft()}.adjusted(1, 2, -4, -2)
                                            : QRectF{rect.topLeft(), grooveRect.topRight()}.adjusted(2, 1, -2, -4);
            const QRectF slider = isActive
                                      ? (isHorizontal
                                             ? QRectF(sliderRect.left(), center - 3, sliderRect.width(), 6)
                                             : QRectF(center - 3, sliderRect.top(), 6, sliderRect.height()))
                                      : isHorizontal
                                            ? QRectF(sliderRect.left(), center - 1, sliderRect.width(), 2)
                                            : QRectF(center - 1, sliderRect.top(), 2, sliderRect.height());
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
            isActive ? drawArrow(isHorizontal) : void();
            painter->restore();
            return;
        }

        QProxyStyle::drawComplexControl(control, option, painter, widget);
    }


    int ScrollBarStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        switch (metric)
        {
            case PM_ScrollBarExtent : // 12 垂直滚动条的宽度和水平滚动条的高度
                return 12;
            case PM_ScrollBarSliderMin : // 18 垂直滚动条滑块的最小高度和水平滚动条滑块的最小宽度
                return 18;
            default :
                break;
        }
        return QProxyStyle::pixelMetric(metric, option, widget);
    }

    void ScrollBarStyle::drawArrow(QPainter *painter, const QRectF &arrowRect, const Direction dir)
    {
        QPainterPath path;
        QPointF p1, p2, p3;
        auto &pal = Palette::palette();
        switch (dir)
        {
            case Direction::Right :
                p1 = arrowRect.topLeft();
                p2 = arrowRect.bottomLeft();
                p3 = QPointF(arrowRect.right(), arrowRect.center().y());
                break;
            case Direction::Left :
                p1 = arrowRect.topRight();
                p2 = arrowRect.bottomRight();
                p3 = QPointF(arrowRect.left(), arrowRect.center().y());
                break;
            case Direction::Down :
                p1 = arrowRect.topLeft();
                p2 = arrowRect.topRight();
                p3 = QPointF(arrowRect.center().x(), arrowRect.bottom());
                break;
            case Direction::Up :
                p1 = arrowRect.bottomLeft();
                p2 = arrowRect.bottomRight();
                p3 = QPointF(arrowRect.center().x(), arrowRect.top());
                break;
            default :
                return;
        }
        path.moveTo(p1);
        path.lineTo(p2);
        path.lineTo(p3);
        path.closeSubpath();
        painter->fillPath(path, pal[ColorRole::ScrollBarSlider]);
        painter->strokePath(path, QPen(pal[ColorRole::ScrollBarSlider], 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }


} // fancy
