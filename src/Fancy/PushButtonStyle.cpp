//
// Created by TX on 2025/9/26.
//

#include "PushButtonStyle.h"

#include <QAbstractButton>
#include <QPainterPath>
#include <QStyleOption>

#include "ControlState.h"
#include "Defs.h"
#include "Palette.h"

namespace fancy
{
    PushButtonStyle::PushButtonStyle(QPushButton *parent) :
        QProxyStyle(nullptr),
        _buttonRadius(3.0),
        _parent(parent)

    {
        setParent(parent);
        connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QPushButton::update));
        connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QPushButton::update));
    }

    void PushButtonStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
    {
        Q_UNUSED(flags)
        Q_UNUSED(pal)
        Q_UNUSED(textRole)
        painter->save();
        painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);
        Palette &palette = Palette::palette();
        painter->setPen(enabled ? (_parent->isChecked() ? palette[ColorRole::AppAccentText] : palette[ColorRole::Text]) : palette[ColorRole::DisEnabledText]);
        painter->drawText(rect, Qt::AlignmentFlag::AlignCenter | Qt::AlignmentFlag::AlignVCenter, text);
        painter->restore();
    }

    void PushButtonStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_PushButtonBevel)
        {
            const QRect rect = QProxyStyle::subElementRect(SE_PushButtonFocusRect, option, widget);
            painter->save();
            painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);
            QPainterPath clipPath; // 圆角矩形裁剪路径
            clipPath.addRoundedRect(rect, _buttonRadius, _buttonRadius);
            Palette &pal = Palette::palette();
            const ControlState state(option->state, _parent->isCheckable() ? ControlType::WidgetCheckable : ControlType::Widget);
            switch (state)
            {
                case VisualState::Normal : [[fallthrough]];
                case VisualState::Pressed : [[fallthrough]];
                case VisualState::Hover :
                    painter->setBrush(pal[ColorRole::Button]);
                    break;
                case VisualState::Selected : [[fallthrough]];
                case VisualState::SelectedPressed : [[fallthrough]];
                case VisualState::SelectedHover :
                    painter->setBrush(pal[ColorRole::AppAccent]);
                    break;
                case VisualState::Disabled : [[fallthrough]];
                case VisualState::SelectedDisabled :
                    painter->setBrush(pal[ColorRole::DisEnabled]);
                    break;
                default :
                    break;
            }
            QPen pen(pal[ColorRole::ButtonBorder]);
            painter->setPen(pen);
            painter->drawPath(clipPath);
            if (state == VisualState::Hover || state == VisualState::SelectedHover)
            {
                painter->setBrush(pal[ColorRole::MaskHover]);
                painter->drawPath(clipPath);
            }
            pen.setColor(pal[ColorRole::ButtonBottomLine]);
            painter->drawLine(bottomLine(rect, _buttonRadius));

            painter->restore();
            return;
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }

    void PushButtonStyle::setRadius(int radius)
    {
        _buttonRadius = radius;
        _parent->update();
    }

    qreal PushButtonStyle::radius() const
    {
        return _buttonRadius;
    }
} // fancy
