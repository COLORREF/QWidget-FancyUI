//
// Created by TX on 2025/9/30.
//

#include "TransparentButtonStyle.h"
#include <QPainterPath>
#include <QPushButton>
#include <QStyleOption>

#include "ControlState.h"
#include "Defs.hpp"
#include "Palette.h"

namespace fancy
{
    TransparentButtonStyle::TransparentButtonStyle(QPushButton *parent) :
        QProxyStyle(nullptr),
        _drawBorder(false),
        _buttonRadius(3),
        _parent(parent)
    {
        setParent(parent);
        connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QPushButton::update));
        connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QPushButton::update));
    }

    void TransparentButtonStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
    {
        Q_UNUSED(flags)
        Q_UNUSED(pal)
        Q_UNUSED(textRole)
        painter->save();
        painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);
        Palette &palette = Palette::palette();
        painter->setPen(palette[enabled ? ColorRole::Text : ColorRole::DisEnabled]);
        painter->drawText(rect, Qt::AlignmentFlag::AlignCenter | Qt::AlignmentFlag::AlignVCenter, text);
        painter->restore();
    }

    void TransparentButtonStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_PushButtonBevel)
        {
            const QRect rect = QProxyStyle::subElementRect(SE_PushButtonFocusRect, option, widget);
            painter->save();
            painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);
            QPainterPath clipPath; // 圆角矩形裁剪路径
            clipPath.addRoundedRect(rect, _buttonRadius, _buttonRadius);
            Palette &pal = Palette::palette();
            switch (const ControlState state(option->state, _parent->isCheckable() ? ControlType::WidgetCheckable : ControlType::Widget); state)
            {
                case VisualState::Normal :
                    painter->setBrush(pal[ColorRole::MaskNormal]);
                    break;
                case VisualState::Pressed :
                    painter->setBrush(pal[ColorRole::MaskPressed]);
                    break;
                case VisualState::Hover :
                    painter->setBrush(pal[ColorRole::MaskHover]);
                    break;
                case VisualState::Selected :
                    painter->setBrush(pal[ColorRole::MaskSelected]);
                    break;
                case VisualState::SelectedHover :
                    painter->setBrush(pal[ColorRole::MaskSelectedHover]);
                    break;
                case VisualState::SelectedPressed :
                    painter->setBrush(pal[ColorRole::MaskSelectedPressed]);
                    break;
                case VisualState::Disabled : [[fallthrough]];
                case VisualState::SelectedDisabled :
                    painter->setBrush(pal[ColorRole::DisEnabled]);
                    break;
                default :
                    break;
            }
            painter->setPen(pal[_drawBorder ? ColorRole::ButtonBorder : ColorRole::MaskNormal]);
            painter->drawPath(clipPath);
            if (_drawBorder)
            {
                painter->setPen(pal[ColorRole::ButtonBottomLine]);
                painter->drawLine(bottomLine(rect, _buttonRadius));
            }
            painter->restore();
            return;
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }
} // fancy
