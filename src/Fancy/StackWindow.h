//
// Created by TX on 2025/10/29.
//

#ifndef QWIDGET_FANCYUI_STACKWINDOW_H
#define QWIDGET_FANCYUI_STACKWINDOW_H
#include "MSWindow.h"
#include "PushButton.h"

namespace fancy
{
    class UniversalTitleBarButton;

    class StackWindow : public MSWindow
    {
        Q_OBJECT

    public:
        explicit StackWindow(QWidget *parent = nullptr);

        void addSidebarOption(SidebarButton *btn) override;

        void setBottomFixedAreaHeight(int height);

        QWidget *bottomFixedArea();

        QPushButton *expendButton();

    private:
        void expandOrRetract(const QVariant &value);

        QWidget *_sidebarArea;
        QWidget *_bottomFixedArea;
        QVBoxLayout *_sidebarLayout;
        UniversalTitleBarButton *_expend;
    };
} // fancy

#endif //QWIDGET_FANCYUI_STACKWINDOW_H
