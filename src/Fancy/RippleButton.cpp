//
// Created by TX on 2025/10/4.
//

#include "RippleButton.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "ControlState.h"
#include "Defs.h"
#include "Icon.h"
#include "Palette.h"
#include "RippleAnimation.h"
#include "RippleButtonStyle.h"

namespace fancy
{
    RippleButton::RippleButton(QWidget *parent, int RippleDuration) :
        QPushButton(parent)
    {
        setStyle(new RippleButtonStyle(this, RippleDuration));
        QFont font = this->font();
        font.setPointSize(12);
        setFont(font);
    }

    RippleButton::RippleButton(const QString &text, QWidget *parent, int RippleDuration) :
        RippleButton(parent, RippleDuration)
    {
        setText(text);
    }

    RippleButton::RippleButton(const QIcon &icon, const QString &text, QWidget *parent, int RippleDuration) :
        RippleButton(text, parent, RippleDuration)
    {
        setIcon(icon);
    }

    RippleButton::RippleButton(const Icon &icon, const QString &text, QWidget *parent, int RippleDuration) :
        RippleButton(text, parent, RippleDuration)
    {
        setIcon(icon);
    }

    RippleButton::RippleButton(const IconId &id, const QString &text, QWidget *parent, int RippleDuration) :
        RippleButton(Icon(id), text, parent, RippleDuration) {}

    void RippleButton::setIcon(const IconId &id)
    {
        setIcon(Icon(id));
    }

    void RippleButton::setIcon(const Icon &icon, bool useAccentTextColor)
    {
        icon.useAccentTextColor(useAccentTextColor);
        QPushButton::setIcon(icon);
    }
} // fancy
