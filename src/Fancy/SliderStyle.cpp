//
// Created by TX on 2025/9/26.
//

#include "SliderStyle.h"
#include <QMouseEvent>
#include <QStyleOption>

#include "Defs.hpp"
#include "Palette.h"
#include "SliderIndicator.h"

namespace fancy
{
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
                else if (event->type() == QEvent::MouseButtonRelease && !_indicator->geometry().contains(mouseEvent->pos()))
                    _indicator->leaveEvent(event);
            }
        }
        return QProxyStyle::eventFilter(obj, event);
    }

    void SliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
    {
        if (control == CC_Slider)
        {
            // 滑槽与指示器区域
            const QRect grooveRect = QProxyStyle::subControlRect(CC_Slider, qstyleoption_cast<const QStyleOptionComplex *>(option), SC_SliderGroove, widget);
            const QRect handleRect = QProxyStyle::subControlRect(CC_Slider, qstyleoption_cast<const QStyleOptionComplex *>(option), SC_SliderHandle, widget);

            auto &palette = Palette::palette();
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);

            constexpr qreal penRatio = 0.278;
            bool isHorizontal = _indicator->_orientation == Qt::Horizontal;

            qreal penWidth = (isHorizontal ? handleRect.height() : handleRect.width()) * penRatio;
            qreal correctXY = penWidth / 2; // 修正画笔圆角半径误差
            qreal center = isHorizontal ? grooveRect.center().y() : grooveRect.center().x();

            QLineF primaryLine = isHorizontal
                                     ? QLineF(correctXY, center, handleRect.left() + correctXY, center)
                                     : QLineF(center, handleRect.bottom() - correctXY, center, grooveRect.bottom() - correctXY);

            QLineF secondaryLine = isHorizontal
                                       ? QLineF(handleRect.right() - correctXY, center, grooveRect.right() - correctXY, center)
                                       : QLineF(center, correctXY, center, handleRect.top() + correctXY);

            // 画线
            QPen pen(palette[ColorRole::AppAccent], penWidth, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap);
            painter->setPen(pen);
            painter->drawLine(primaryLine);
            pen.setColor(palette[ColorRole::DisEnabled].darker(120));
            painter->setPen(pen);
            painter->drawLine(secondaryLine);


            // 更新指示器位置
            int side = isHorizontal ? grooveRect.height() : grooveRect.width();

            int offset = (isHorizontal
                              ? handleRect.x() - (side - handleRect.width()) / 2
                              : handleRect.y() - (side - handleRect.height()) / 2);
            if (offset < 0)
                offset = 0;
            else if (isHorizontal && offset + side > widget->width())
                offset = widget->width() - grooveRect.height();

            else if (!isHorizontal && offset + side > widget->height())
                offset = widget->height() - grooveRect.width();
            QRect indRect = (isHorizontal
                                 ? QRect(QPoint(offset, 0), QSize(grooveRect.height(), grooveRect.height()))
                                 : QRect(QPoint(0, offset), QSize(grooveRect.width(), grooveRect.width())));

            if (_indicator->geometry() != indRect)
                _indicator->setGeometry(indRect);

            painter->restore();
            return;
        }

        return QProxyStyle::drawComplexControl(control, option, painter, widget);
    }
} // fancy
