//
// Created by TX on 2025/9/29.
//

#include "ThemePushButtonStyle.h"
#include <QPainterPath>
#include <QPushButton>
#include <QStyleOption>

#include "ControlState.h"
#include "Defs.h"
#include "Palette.h"

namespace fancy
{
    ThemeButtonStyle::ThemeButtonStyle(QPushButton *parent) :
        QProxyStyle(nullptr),
        _buttonRadius(3)
    {
        setParent(parent);
        connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QPushButton::update));
        connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QPushButton::update));
    }

    void ThemeButtonStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
    {
        Q_UNUSED(flags)
        Q_UNUSED(pal)
        Q_UNUSED(textRole)
        painter->save();
        painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);
        Palette &palette = Palette::palette();
        painter->setPen(enabled ? palette[ColorRole::AppAccentText] : palette[ColorRole::DisEnabled]);
        painter->drawText(rect, Qt::AlignmentFlag::AlignCenter | Qt::AlignmentFlag::AlignVCenter, text);
        painter->restore();
    }

    void ThemeButtonStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_PushButtonBevel)
        {
            QRect rect = QProxyStyle::subElementRect(SE_PushButtonFocusRect, option, widget);
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            QPainterPath clipPath; // 圆角矩形裁剪路径
            Palette &pal = Palette::palette();
            clipPath.addRoundedRect(rect, _buttonRadius, _buttonRadius);

            const ControlState state(option->state, ControlType::Widget);
            switch (state)
            {
                case VisualState::Normal : [[fallthrough]];
                case VisualState::Pressed : [[fallthrough]];
                case VisualState::Hover :
                    painter->setBrush(pal[ColorRole::AppAccent]);
                    break;
                case VisualState::Disabled :
                    painter->setBrush(pal[ColorRole::DisEnabled]);
                    break;
                default :
                    break;
            }

            QPen pen(pal[ColorRole::ButtonBorder]);
            painter->setPen(pen);
            painter->drawPath(clipPath);
            if (state == VisualState::Hover)
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

    void ThemeButtonStyle::setRadius(int radius)
    {
        _buttonRadius = radius;
        if (auto *par = qobject_cast<QPushButton *>(parent()))
            par->update();
    }

    qreal ThemeButtonStyle::radius() const
    {
        return _buttonRadius;
    }
} // fancy
