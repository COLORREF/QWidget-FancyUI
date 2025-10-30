//
// Created by TX on 2025/9/30.
//

#ifndef QWIDGET_FANCYUI_THEMEBUTTON_H
#define QWIDGET_FANCYUI_THEMEBUTTON_H
#include <QPushButton>
#include "Defs.hpp"

namespace fancy
{
    class Icon;
    enum class BootstrapIcons : unsigned int;
    enum class AntDesignIcons : unsigned int;
    enum class IconPark : unsigned int;
    struct IconId;

    class ThemeButton : public QPushButton
    {
        Q_OBJECT

    public:
        explicit ThemeButton(QWidget *parent = nullptr);

        explicit ThemeButton(const QString &text, QWidget *parent = nullptr);

        ThemeButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

        ThemeButton(const Icon &icon, const QString &text, QWidget *parent = nullptr);

        ThemeButton(const IconId &id, const QString &text, QWidget *parent = nullptr);

#define THEMEBUTTON_ICON_ENUM_CONSTRUCTOR(ENUM_TYPE) ThemeButton(ENUM_TYPE iconEnum, const QString &text, QWidget *parent = nullptr):\
ThemeButton(iconId(iconEnum), text, parent) {}

        THEMEBUTTON_ICON_ENUM_CONSTRUCTOR(AntDesignIcons)
        THEMEBUTTON_ICON_ENUM_CONSTRUCTOR(BootstrapIcons)
        THEMEBUTTON_ICON_ENUM_CONSTRUCTOR(IconPark)

        using QPushButton::setIcon;

        void setIcon(const IconId &id);

        void setIcon(const Icon &icon, bool useAccentTextColor = true);

#define THEMEBUTTON_SET_ICON(ENUM_TYPE) void setIcon(ENUM_TYPE iconEnum){setIcon(iconId(iconEnum));}

        THEMEBUTTON_SET_ICON(AntDesignIcons)
        THEMEBUTTON_SET_ICON(BootstrapIcons)
        THEMEBUTTON_SET_ICON(IconPark)
    };
} // fancy

#endif //QWIDGET_FANCYUI_THEMEBUTTON_H
