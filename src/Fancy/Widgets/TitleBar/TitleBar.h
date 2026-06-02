//
// Created by TX on 2025/9/19.
//

#ifndef QWIDGET_FANCYUI_TITLEBAR_H
#define QWIDGET_FANCYUI_TITLEBAR_H

#include <QLabel>
#include <QWidget>


class QSpacerItem;
class QHBoxLayout;

namespace fancy
{
    class TextLabel;
    class MaximizeButton;
    class MinimizeButton;
    class CloseButton;
    struct IconId;

    class TitleBar : public QWidget
    {
        Q_OBJECT

        friend class Window;
        friend class MSWindow;
        friend class StackWindow;

    public:
        explicit TitleBar(QWidget *parent);

        void setIcon(const QPixmap &icon);

        void setTitleText(const QString &text);

        // void setTitleTextColor(const QColor &color);

        void setTitleTextFont(const QFont &font);

        void removeIconLabel();

        void removeTextLabel();

        void removeMinimizeButton();

        void removeMaximizeButton();

        void removeCloseButton();

        void setDoubleClickMaximize(bool allow);

        void resetTitleBtnFixedSize(const QSize &size);

        [[nodiscard]] QSize titleBtnFixedSize() const;

        void insertWidget(int index, QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());

    protected:
        void mousePressEvent(QMouseEvent *event) override;

        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void setTransparency(bool set = true) const;

    private:
        bool _doubleClickMaximize;
        QHBoxLayout *_horizontalLayout;
        CloseButton *_closeButton;
        MaximizeButton *_maximizeButton;
        MinimizeButton *_minimizeButton;
        QSpacerItem *_horizontalSpacer;
        TextLabel *_textLabel;
        QLabel *_iconLabel;
    };
}


#endif //QWIDGET_FANCYUI_TITLEBAR_H
