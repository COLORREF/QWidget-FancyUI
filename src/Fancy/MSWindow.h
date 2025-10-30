//
// Created by TX on 2025/10/13.
//

#ifndef QWIDGET_FANCYUI_MSWINDOW_H
#define QWIDGET_FANCYUI_MSWINDOW_H
#include "Window.h"


class QStackedWidget;

namespace fancy
{
    class SidebarButton;
    class MSWidget;
    class Sidebar;

    class MSWindow : public Window
    {
        Q_OBJECT
        friend class StackWindow;

    public:
        explicit MSWindow(QWidget *parent = nullptr);

        QWidget *setupUiWidget() = delete;

        void addWidget(QWidget *widget);

        void setCurrentIndex(int index);

        virtual void addSidebarOption(SidebarButton *btn);

        [[nodiscard]] Sidebar *sidebar() const { return _sidebar; }
        [[nodiscard]] QStackedWidget *stackWidget() const { return _stackWidget; }

    private:
        QHBoxLayout *_globalLayout;
        MSWidget *_msClientArea;
        QHBoxLayout *_clientLayout;
        Sidebar *_sidebar;
        QStackedWidget *_stackWidget;
    };
} // fancy

#endif //QWIDGET_FANCYUI_MSWINDOW_H
