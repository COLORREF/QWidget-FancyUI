//
// Created by TX on 2025/9/30.
//

#ifndef QWIDGET_FANCYUI_TRANSPARENTBUTTON_H
#define QWIDGET_FANCYUI_TRANSPARENTBUTTON_H
#include <QPushButton>

#include "Defs.hpp"

namespace fancy
{
    class Icon;
    enum class BootstrapIcons : unsigned int;
    enum class AntDesignIcons : unsigned int;
    enum class IconPark : unsigned int;
    struct IconId;

    class TransparentButton : public QPushButton
    {
        Q_OBJECT

    public:
        explicit TransparentButton(QWidget *parent = nullptr);

        explicit TransparentButton(const QString &text, QWidget *parent = nullptr);

        TransparentButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

        TransparentButton(const Icon &icon, const QString &text, QWidget *parent = nullptr);

        TransparentButton(const IconId &id, const QString &text, QWidget *parent = nullptr);

        void setDrawBorder(bool draw);

        void setRadius(int radius);

#define TRANSPARENTBUTTON_ICON_ENUM_CONSTRUCTOR(ENUM_TYPE) TransparentButton(ENUM_TYPE iconEnum, const QString &text, QWidget *parent = nullptr):\
TransparentButton(iconId(iconEnum), text, parent) {}

        TRANSPARENTBUTTON_ICON_ENUM_CONSTRUCTOR(AntDesignIcons)
        TRANSPARENTBUTTON_ICON_ENUM_CONSTRUCTOR(BootstrapIcons)
        TRANSPARENTBUTTON_ICON_ENUM_CONSTRUCTOR(IconPark)

        using QPushButton::setIcon;

        void setIcon(const IconId &id);

        void setIcon(const Icon &icon);

#define TRANSPARENTBUTTON_SET_ICON(ENUM_TYPE) void setIcon(ENUM_TYPE iconEnum){setIcon(iconId(iconEnum));}

        TRANSPARENTBUTTON_SET_ICON(AntDesignIcons)
        TRANSPARENTBUTTON_SET_ICON(BootstrapIcons)
        TRANSPARENTBUTTON_SET_ICON(IconPark)
    };
} // fancy

#endif //QWIDGET_FANCYUI_TRANSPARENTBUTTON_H
