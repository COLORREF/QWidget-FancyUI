//
// Created by TX on 2025/9/29.
//

#include "PushButton.h"
#include "Icon.h"
#include "PushButtonStyle.h"

namespace fancy
{
    PushButton::PushButton(QWidget *parent) :
        QPushButton(parent)
    {
        setStyle(new PushButtonStyle(this));
        QFont font = this->font();
        font.setPixelSize(15);
        setFont(font);
    }

    PushButton::PushButton(const QString &text, QWidget *parent) :
        PushButton(parent)
    {
        setText(text);
    }

    PushButton::PushButton(const QIcon &icon, const QString &text, QWidget *parent) :
        PushButton(text, parent)
    {
        setIcon(icon);
    }

    PushButton::PushButton(const Icon &icon, const QString &text, QWidget *parent) :
        PushButton(text, parent)
    {
        setIcon(icon);
    }

    PushButton::PushButton(const IconId &id, const QString &text, QWidget *parent) :
        PushButton(Icon(id), text, parent) {}

    void PushButton::setIcon(const IconId &id)
    {
        setIcon(Icon(id));
    }

    void PushButton::setIcon(const Icon &icon, bool checkedUseAccentTextColor)
    {
        icon.stateOnUseAccentTextColor(checkedUseAccentTextColor);
        QPushButton::setIcon(icon);
    }
} // fancy
