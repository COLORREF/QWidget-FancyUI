//
// Created by TX on 2025/10/5.
//

#include "RippleButtonStyle.h"
#include <QMouseEvent>
#include <QPainterPath>
#include <QPushButton>
#include <QStyleOption>

#include "ControlState.h"
#include "Defs.hpp"
#include "Palette.h"
#include "RippleAnimation.h"

namespace fancy
{
    RippleButtonStyle::RippleButtonStyle(QPushButton *parent, int RippleDuration) :
        QProxyStyle(nullptr),
        _parent(parent),
        _buttonRadius(3)
    {
        setParent(parent);
        parent->installEventFilter(this);
        int maxRadius = static_cast<int>(qSqrt(qPow(parent->width() / 2, 2) + qPow(parent->height() / 2, 2)));
        auto &pal = Palette::palette();
        const QColor start = Palette::mix(Qt::GlobalColor::white, pal[ColorRole::AppAccent], 0.3f);
        const QColor end = Palette::palette()[ColorRole::AppAccent];

        _ripple = new RippleAnimation(RippleDuration, maxRadius, start, end, 7, this);
        _ripple->setUpdate(parent);

        auto updateColor = [this,&pal] {
            _ripple->updateStartColor(Palette::mix(Qt::GlobalColor::white, pal[ColorRole::AppAccent]));
            _ripple->updateEndColor(pal[ColorRole::AppAccent]);
            _parent->update();
        };

        installEventFilter(this);
        connect(&Palette::palette(), &Palette::appThemeChange, this, updateColor);
        connect(&Palette::palette(), &Palette::appColorChange, this, updateColor);
    }

    void RippleButtonStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
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

    void RippleButtonStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_PushButtonBevel)
        {
            QRect rect = QProxyStyle::subElementRect(SE_PushButtonFocusRect, option, widget);
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            QPainterPath clip_path; // 圆角矩形裁剪路径
            clip_path.addRoundedRect(rect, _buttonRadius, _buttonRadius);
            Palette &pal = Palette::palette();
            const ControlState state(option->state, _parent->isCheckable() ? ControlType::WidgetCheckable : ControlType::Widget);

            switch (state)
            {
                case VisualState::Normal : [[fallthrough]];
                case VisualState::Pressed : [[fallthrough]];
                case VisualState::Hover :
                    painter->setBrush(_parent->isCheckable() ? pal[ColorRole::Button] : pal[ColorRole::AppAccent]);
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
            painter->setClipPath(clip_path);
            QPen pen(pal[ColorRole::ButtonBorder]);
            painter->setPen(pen);
            painter->drawPath(clip_path);
            if (state == VisualState::Hover || state == VisualState::SelectedHover)
            {
                painter->setBrush(pal[ColorRole::MaskHover]);
                painter->drawPath(clip_path);
            }
            pen.setColor(pal[ColorRole::ButtonBottomLine]);
            painter->drawLine(bottomLine(rect, _buttonRadius));
            if (!_parent->isCheckable())
                _ripple->paint(painter);
            painter->restore();
            return;
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }

    bool RippleButtonStyle::eventFilter(QObject *obj, QEvent *event)
    {
        if (obj != _parent)
            return QProxyStyle::eventFilter(obj, event);

        if (event->type() == QEvent::Type::Resize)
            _ripple->updateMaxRadius(static_cast<int>(qSqrt(qPow(_parent->width() / 2, 2) + qPow(_parent->height() / 2, 2))));

        if (_parent->isEnabled() && (event->type() == QEvent::Type::MouseButtonPress || event->type() == QEvent::Type::MouseButtonDblClick))
            if (const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event); mouseEvent->button() == Qt::MouseButton::LeftButton)
                _ripple->start(mouseEvent->position());

        return QProxyStyle::eventFilter(obj, event);
    }
} // fancy
