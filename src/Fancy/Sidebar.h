//
// Created by TX on 2025/10/10.
//

#ifndef QWIDGET_FANCYUI_SIDEBAR_H
#define QWIDGET_FANCYUI_SIDEBAR_H

#include <QQueue>
#include  <QWidget>

class QVariantAnimation;
class QAbstractButton;
class QSpacerItem;
class QButtonGroup;
class QVBoxLayout;

namespace fancy
{
    class SidebarButton;
    class ScrollArea;

    constexpr int LINE_WIDTH = 3;
    constexpr int ContentsMargins_Left = 2;
    constexpr int ContentsMargins_Top = 5;
    constexpr int ContentsMargins_Right = 2;
    constexpr int ContentsMargins_Bottom = 5;

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

        void addOptionToLayout(SidebarButton *option);

        void setExpandRetractDuration(int msecs);

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
    signals:
        void optionChecked(int id);

    private slots:
        void startIndicatorAnimation(int id, bool checked);

        void expandOrRetract(const QVariant &value);
    };
} // fancy

#endif //QWIDGET_FANCYUI_SIDEBAR_H
