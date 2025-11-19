//
// Created by TX on 2025/10/10.
//

#ifndef QWIDGET_FANCYUI_SIDEBAR_H
#define QWIDGET_FANCYUI_SIDEBAR_H

#include <QQueue>
#include <QWidget>

class QVariantAnimation;
class QAbstractButton;
class QSpacerItem;
class QButtonGroup;
class QVBoxLayout;

namespace fancy
{
    class SidebarButton;
    class ScrollArea;

    constexpr int Sidebar_LINE_WIDTH = 3;
    constexpr int Sidebar_Spacing = 2;
    constexpr int Sidebar_ContentsMargins_Left = 2;
    constexpr int Sidebar_ContentsMargins_Top = 5;
    constexpr int Sidebar_ContentsMargins_Right = 2;
    constexpr int Sidebar_ContentsMargins_Bottom = 5;

    class Sidebar : public QWidget
    {
        Q_OBJECT
        friend class StackWindow;

    public:
        explicit Sidebar(QWidget *parent);

        void addOption(SidebarButton *option);

        void expand();

        void retract();

        void autoExpandRetract();

        [[nodiscard]] bool isExpanded() const { return _isExpanded; }

        void setMinimumWidth(int width);

        void setMaximumWidth(int width);

        void setFixedWidth(int width);

        int maximumWidth();

        int minimumWidth();

        void addOptionToGroup(SidebarButton *option);

        void removeOptionFromGroup(SidebarButton *option);

        void addOptionToLayout(SidebarButton *option);

        void setExpandRetractDuration(int msecs);

        void hideScrollbar();

        void restoreScrollBarPolicy();

        [[nodiscard]] int buttonCount();

    private:
        QVBoxLayout *_verticalLayout;
        QButtonGroup *_btnGroup;
        QSpacerItem *_verticalSpacer;
        ScrollArea *_scrollArea;
        QWidget *_scrollContentWidget;
        QVBoxLayout *_scrollContentWidgetVerticalLayout;
        QQueue<int> _checked;
        QQueue<int> _unChecked;
        QVariantAnimation *_expandAndRetractAni;
        bool _isExpanded;
        int _level;
    signals:
        void optionChecked(int id);

        /**
         * 展开或者收缩结束后发送此信号
         */
        void expandStateChange(bool isExpanded);

        void startExpand();

        void startRetract();

    private slots:
        void startIndicatorAnimation(int id, bool checked);

        void expandOrRetract(const QVariant &value);

        void expandOrRetractFinished();
    };
} // fancy

#endif //QWIDGET_FANCYUI_SIDEBAR_H
