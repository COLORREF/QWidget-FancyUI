//
// Created by TX on 2025/9/29.
//

#ifndef QWIDGET_FANCYUI_PUSHBUTTON_H
#define QWIDGET_FANCYUI_PUSHBUTTON_H
#include <QPushButton>

#include "Defs.hpp"

namespace fancy
{
    class Icon;
    enum class BootstrapIcons : unsigned int;
    enum class AntDesignIcons : unsigned int;
    enum class IconPark : unsigned int;
    struct IconId;

    class PushButton : public QPushButton
    {
        Q_OBJECT

    public:
        explicit PushButton(QWidget *parent = nullptr);

        explicit PushButton(const QString &text, QWidget *parent = nullptr);

        PushButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

        PushButton(const Icon &icon, const QString &text, QWidget *parent = nullptr);

        PushButton(const IconId &id, const QString &text, QWidget *parent = nullptr);

#define PUSHBUTTON_ICON_ENUM_CONSTRUCTOR(ENUM_TYPE) PushButton(ENUM_TYPE iconEnum, const QString &text, QWidget *parent = nullptr):\
        PushButton(iconId(iconEnum), text, parent) {}

        PUSHBUTTON_ICON_ENUM_CONSTRUCTOR(AntDesignIcons)
        PUSHBUTTON_ICON_ENUM_CONSTRUCTOR(BootstrapIcons)
        PUSHBUTTON_ICON_ENUM_CONSTRUCTOR(IconPark)

        using QPushButton::setIcon;

        void setIcon(const IconId &id);

        void setIcon(const Icon &icon, bool checkedUseAccentTextColor = true);

#define PUSHBUTTON_SET_ICON(ENUM_TYPE) void setIcon(ENUM_TYPE iconEnum){setIcon(iconId(iconEnum));}

        PUSHBUTTON_SET_ICON(AntDesignIcons)
        PUSHBUTTON_SET_ICON(BootstrapIcons)
        PUSHBUTTON_SET_ICON(IconPark)
    };
} // fancy

#endif //QWIDGET_FANCYUI_PUSHBUTTON_H
