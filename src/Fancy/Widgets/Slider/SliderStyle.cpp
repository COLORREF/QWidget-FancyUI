//
// Created by TX on 2025/9/26.
//

#include "SliderStyle.h"
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include "SliderIndicator.h"
#include "Core/Defs.h"
#include "Core/Palette/Palette.h"

namespace fancy
{
    namespace
    {
        // 逻辑像素，由 Qt 在绘制时应用 DPI 缩放。
        constexpr int indicatorExtent = 21;
        constexpr qreal borderSpace = 1.0;
        constexpr qreal trackRatio = 4.0 / 20.0;
    }

    SliderStyle::SliderStyle(QSlider *parent, Qt::Orientation orientation) :
        QProxyStyle(nullptr),
        _indicator(new SliderIndicator(parent))
    {
        parent->installEventFilter(this);
        setParent(parent);
        _indicator->_orientation = orientation;

        connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QSlider::update));
        connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QSlider::update));
    }

    void SliderStyle::setOrientation(Qt::Orientation orientation)
    {
        _indicator->_orientation = orientation;
    }

    bool SliderStyle::eventFilter(QObject *obj, QEvent *event)
    {
        if (obj == parent() && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease))
        {
            if (auto *mouseEvent = dynamic_cast<QMouseEvent *>(event))
            {
                if (mouseEvent->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress)
                    _indicator->mousePressEvent(mouseEvent);
                else if (event->type() == QEvent::MouseButtonRelease)
                {
                    _indicator->mouseReleaseEvent(mouseEvent);
                    if (!_indicator->geometry().contains(mouseEvent->pos()))
                        _indicator->leaveEvent(event);
                }
            }
        }
        return QProxyStyle::eventFilter(obj, event);
    }

    int SliderStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        if (metric == PM_SliderLength || metric == PM_SliderThickness || metric == PM_SliderControlThickness)
            return indicatorExtent;
        if (metric == PM_SliderSpaceAvailable)
        {
            if (const auto *slider = qstyleoption_cast<const QStyleOptionSlider *>(option))
            {
                const int side = qMin(indicatorExtent, qMin(slider->rect.width(), slider->rect.height()));
                return qMax(0, (slider->orientation == Qt::Horizontal ? slider->rect.width() : slider->rect.height()) - side);
            }
        }
        return QProxyStyle::pixelMetric(metric, option, widget);
    }

    QRect SliderStyle::subControlRect(ComplexControl control, const QStyleOptionComplex *option,
                                     SubControl subControl, const QWidget *widget) const
    {
        const auto *slider = qstyleoption_cast<const QStyleOptionSlider *>(option);
        if (control != CC_Slider || !slider || (subControl != SC_SliderHandle && subControl != SC_SliderGroove))
            return QProxyStyle::subControlRect(control, option, subControl, widget);

        const QRect rect = slider->rect;
        const int side = qMax(0, qMin(indicatorExtent, qMin(rect.width(), rect.height())));
        const bool horizontal = slider->orientation == Qt::Horizontal;
        const int length = horizontal ? rect.width() : rect.height();
        const int offset = sliderPositionFromValue(slider->minimum, slider->maximum, slider->sliderPosition,
                                                  qMax(0, length - side), slider->upsideDown);
        const QRect groove = horizontal
            ? QRect(rect.x(), rect.y() + (rect.height() - side) / 2, rect.width(), side)
            : QRect(rect.x() + (rect.width() - side) / 2, rect.y(), side, rect.height());
        if (subControl == SC_SliderGroove)
            return groove;
        return horizontal ? QRect(groove.x() + offset, groove.y(), side, side)
                          : QRect(groove.x(), groove.y() + offset, side, side);
    }

    void SliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
    {
        if (control == CC_Slider)
        {
            // 滑槽与指示器区域
            const QRect grooveRect = subControlRect(CC_Slider, option, SC_SliderGroove, widget);
            const QRect handleRect = subControlRect(CC_Slider, option, SC_SliderHandle, widget);

            auto &palette = Palette::palette();
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);

            const auto *slider = qstyleoption_cast<const QStyleOptionSlider *>(option);
            if (!slider || handleRect.isEmpty())
            {
                painter->restore();
                return;
            }
            const bool isHorizontal = slider->orientation == Qt::Horizontal;
            const qreal diameter = qMax(0.0, handleRect.width() - borderSpace);
            if (diameter <= 0)
            {
                _indicator->setGeometry(handleRect);
                painter->restore();
                return;
            }
            const qreal penWidth = diameter * trackRatio;
            const qreal capRadius = penWidth / 2;
            const QRectF groove(grooveRect);
            const QPointF handleCenter = QRectF(handleRect).center();
            const QPointF start = isHorizontal ? QPointF(groove.left() + capRadius, handleCenter.y())
                                               : QPointF(handleCenter.x(), groove.top() + capRadius);
            const QPointF end = isHorizontal ? QPointF(groove.right() - capRadius, handleCenter.y())
                                             : QPointF(handleCenter.x(), groove.bottom() - capRadius);
            const QLineF primaryLine(slider->upsideDown ? end : start, handleCenter);
            const QLineF secondaryLine(handleCenter, slider->upsideDown ? start : end);

            // 画线
            QPen pen(palette[ColorRole::AppAccent], penWidth, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap);
            painter->setPen(pen);
            painter->drawLine(primaryLine);
            pen.setColor(palette[ColorRole::DisEnabled].darker(120));
            painter->setPen(pen);
            painter->drawLine(secondaryLine);


            // 绘制和鼠标命中使用同一个矩形，横竖方向共用尺寸基准。
            if (_indicator->geometry() != handleRect)
                _indicator->setGeometry(handleRect);

            painter->restore();
            return;
        }

        return QProxyStyle::drawComplexControl(control, option, painter, widget);
    }
} // fancy
