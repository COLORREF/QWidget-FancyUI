//
// Created by TX on 2025/10/4.
//

#ifndef QWIDGET_FANCYUI_RIPPLEBUTTON_H
#define QWIDGET_FANCYUI_RIPPLEBUTTON_H

#include <QPushButton>

#include "ThemeButton.h"


namespace fancy
{
    class RippleAnimation;

    class RippleButton : public QPushButton
    {
        Q_OBJECT

    public:
        explicit RippleButton(QWidget *parent = nullptr, int RippleDuration = 700);

        explicit RippleButton(const QString &text, QWidget *parent = nullptr, int RippleDuration = 700);

        RippleButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr, int RippleDuration = 700);

        RippleButton(const Icon &icon, const QString &text, QWidget *parent = nullptr, int RippleDuration = 700);

        RippleButton(const IconId &id, const QString &text, QWidget *parent = nullptr, int RippleDuration = 700);

#define RIPPLEBUTTON_ICON_ENUM_CONSTRUCTOR(ENUM_TYPE) RippleButton(ENUM_TYPE iconEnum, const QString &text, QWidget *parent = nullptr, int RippleDuration = 700):\
RippleButton(iconId(iconEnum), text, parent, RippleDuration) {}

        RIPPLEBUTTON_ICON_ENUM_CONSTRUCTOR(AntDesignIcons)
        RIPPLEBUTTON_ICON_ENUM_CONSTRUCTOR(BootstrapIcons)
        RIPPLEBUTTON_ICON_ENUM_CONSTRUCTOR(IconPark)

        using QPushButton::setIcon;

        void setIcon(const IconId &id);

        void setIcon(const Icon &icon, bool useAccentTextColor = true);

#define RIPPLEBUTTON_SET_ICON(ENUM_TYPE) void setIcon(ENUM_TYPE iconEnum){setIcon(iconId(iconEnum));}

        RIPPLEBUTTON_SET_ICON(AntDesignIcons)
        RIPPLEBUTTON_SET_ICON(BootstrapIcons)
        RIPPLEBUTTON_SET_ICON(IconPark)
    };
} // fancy

#endif //QWIDGET_FANCYUI_RIPPLEBUTTON_H
