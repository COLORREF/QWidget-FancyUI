//
// Created by TX on 2025/11/10.
//

#include "PlainTextEditStyle.h"

#include <QPainterPath>
#include <QPlainTextEdit>
#include <QStyleOption>
#include <QVariantAnimation>

#include "ControlState.h"
#include "Defs.h"
#include "palette.h"

namespace fancy
{
    PlainTextEditStyle::PlainTextEditStyle(QPlainTextEdit *parent) :
        QProxyStyle(nullptr),
        _radius(5),
        _parent(parent),
        _ani(new QVariantAnimation(parent))
    {
        setParent(parent);
        _parent->installEventFilter(this);
        _ani->setDuration(250);
        connect(_ani, &QVariantAnimation::valueChanged, this, &PlainTextEditStyle::setIndicator);
    }

    void PlainTextEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        const ControlState state(option->state, ControlType::TextEdit);

        if (element == PE_Frame)
        {
            painter->save();
            switch (state)
            {
                case VisualState::Normal :
                    drawBase(painter, option->rect, ColorRole::TextEditPanelNormal);
                    break;
                case VisualState::Hover :
                    drawBase(painter, option->rect, ColorRole::TextEditPanelHover);
                    break;
                case VisualState::Disabled :
                    if (state._over)
                    {
                        drawBase(painter, option->rect, ColorRole::TextEditPanelHover);
                        break;
                    }
                case VisualState::Focus :
                case VisualState::FocusHover :
                    drawBase(painter, option->rect, ColorRole::TextEditPanelFocus);
                default :
                    break;
            }
            painter->restore();
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

    bool PlainTextEditStyle::eventFilter(QObject *object, QEvent *event)
    {
        if (object == _parent)
        {
            if (event->type() == QEvent::FocusIn)
            {
                const QLine endLine = bottomLine(_parent->rect(), 0);
                const QPoint center = endLine.center();
                const QLine startLine{center, center};
                _ani->setStartValue(startLine);
                _ani->setEndValue(endLine);
                _ani->start();
            }
            else if (event->type() == QEvent::FocusOut)
            {
                const QLine startLine = bottomLine(_parent->rect(), 0);
                const QPoint center = startLine.center();
                const QLine endLine{center, center};
                _ani->setStartValue(startLine);
                _ani->setEndValue(endLine);
                _ani->start();
            }
        }
        return QProxyStyle::eventFilter(object, event);
    }

    void PlainTextEditStyle::setIndicator(const QVariant &val)
    {
        _indicator = val.toLine();
        _parent->update();
    }

    void PlainTextEditStyle::drawBase(QPainter *painter, const QRect &rect, const ColorRole fillRole) const
    {
        auto &pal = Palette::palette();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen({pal[ColorRole::TextEditBorder], 1});
        const QRectF rectF = QRectF(rect).adjusted(0.5, 0.5, -0.5, -0.5);
        QPainterPath path;
        path.addRoundedRect(rectF, _radius, _radius);
        painter->setClipPath(path);
        painter->fillPath(path, pal[fillRole]);
        painter->strokePath(path, {pal[ColorRole::TextEditBorder], 1});
        painter->translate(0, -1);
        painter->setPen({pal[ColorRole::TextEditIndicatorLine], 0.5});
        painter->drawLine(bottomLine(rect, 0));
        if (_indicator.x1() != _indicator.x2())
        {
            painter->setPen({pal[ColorRole::AppAccent], 2});
            painter->drawLine(_indicator);
        }
    }
} // fancy
