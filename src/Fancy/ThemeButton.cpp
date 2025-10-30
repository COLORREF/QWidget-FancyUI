//
// Created by TX on 2025/9/30.
//

#include "ThemeButton.h"

#include "Icon.h"
#include "ThemePushButtonStyle.h"

namespace fancy
{
    ThemeButton::ThemeButton(QWidget *parent) :
        QPushButton(parent)
    {
        setStyle(new ThemeButtonStyle(this));
        QFont font = this->font();
        font.setPixelSize(15);
        setFont(font);
    }

    ThemeButton::ThemeButton(const QString &text, QWidget *parent) :
        ThemeButton(parent)
    {
        setText(text);
    }

    ThemeButton::ThemeButton(const QIcon &icon, const QString &text, QWidget *parent) :
        ThemeButton(text, parent)
    {
        setIcon(icon);
    }

    ThemeButton::ThemeButton(const Icon &icon, const QString &text, QWidget *parent) :
        ThemeButton(text, parent)
    {
        setIcon(icon);
    }

    ThemeButton::ThemeButton(const IconId &id, const QString &text, QWidget *parent) :
        ThemeButton(Icon(id), text, parent) {}

    void ThemeButton::setIcon(const IconId &id)
    {
        setIcon(Icon(id));
    }

    void ThemeButton::setIcon(const Icon &icon, bool useAccentTextColor)
    {
        icon.useAccentTextColor(useAccentTextColor);
        QPushButton::setIcon(icon);
    }
} // fancy
