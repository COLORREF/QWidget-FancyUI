//
// Created by TX on 2025/10/7.
//

#ifndef QWIDGET_FANCYUI_SIDEBARBUTTON_H
#define QWIDGET_FANCYUI_SIDEBARBUTTON_H
#include <QLabel>

#include "TransparentButton.h"


class QVariantAnimation;
class QLabel;

namespace fancy
{
    class SvgWidget;

    class SidebarButton : public TransparentButton
    {
        Q_OBJECT
        friend class Sidebar;
        friend class SidebarNode;

    public:
        explicit SidebarButton(QWidget *parent, int sidebarWidth = 40);

        explicit SidebarButton(const QString &text, QWidget *parent, int sidebarWidth = 40);

        explicit SidebarButton(IconId id, const QString &text, QWidget *parent, int sidebarWidth = 40);

#define SIDEBARBUTTON_ICON_ENUM_CONSTRUCTOR(ENUM_TYPE) SidebarButton(ENUM_TYPE iconEnum, const QString &text, QWidget *parent = nullptr, int sidebarWidth = 40):\
SidebarButton(iconId(iconEnum), text, parent, sidebarWidth) {}

        SIDEBARBUTTON_ICON_ENUM_CONSTRUCTOR(AntDesignIcons)
        SIDEBARBUTTON_ICON_ENUM_CONSTRUCTOR(BootstrapIcons)
        SIDEBARBUTTON_ICON_ENUM_CONSTRUCTOR(IconPark)

        void setIcon(const QPixmap &icon);

        void setIcon(const QPicture &icon);

        void setIcon(IconId id);

#define SIDEBARBUTTON_SET_ICON(ENUM_TYPE) void setIcon(ENUM_TYPE iconEnum){setIcon(iconId(iconEnum));}

        SIDEBARBUTTON_SET_ICON(AntDesignIcons)
        SIDEBARBUTTON_SET_ICON(BootstrapIcons)
        SIDEBARBUTTON_SET_ICON(IconPark)

        void setText(const QString &text);

        void setFont(const QFont &font);

        void setSidebarWidth(int sidebarWidth);

        [[nodiscard]] int sidebarWidth() const;

        void setDrawIndicator(bool drawIndicator) { _drawIndicator = drawIndicator; }

        void setAdjustIconCoordinates(bool adjust) { _adjustIconCoordinates = adjust; }

        [[nodiscard]] bool drawIndicator() const { return _drawIndicator; }
        [[nodiscard]] bool adjustIconCoordinates() const { return _adjustIconCoordinates; }

        [[nodiscard]] QString text() const { return _textLabel->text(); };

    protected:
        void paintEvent(QPaintEvent *event) override;

        void setLeftOffset(int offset);

    private:
        SvgWidget *_svgWidget;
        QLabel *_imageWidget;
        QLabel *_textLabel;
        QLine _indicator;
        QLine _upIndicator;
        QLine _unIndicator;
        QVariantAnimation *_topToMiddle;
        QVariantAnimation *_middleToBottom;
        int _leftOffset;
        bool _drawIndicator;
        bool _adjustIconCoordinates;

    private slots:
        void setIndicator(const QVariant &value);

        void correctIconCoordinates(bool checked); //修正图标坐标
    };
}


#endif //QWIDGET_FANCYUI_SIDEBARBUTTON_H
