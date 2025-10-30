//
// Created by TX on 2025/10/29.
//

#include "StackWindow.h"

#include <QHBoxLayout>
#include <QVariantAnimation>

#include "CloseButton.h"
#include "MaximizeButton.h"
#include "MinimizeButton.h"
#include "PushButton.h"
#include "Sidebar.h"
#include "TitleBar.h"
#include "TransparentButton.h"

namespace fancy
{
    StackWindow::StackWindow(QWidget *parent) :
        MSWindow(parent),
        _sidebarArea(new QWidget(this)),
        _bottomFixedArea(new QWidget(this)),
        _sidebarLayout(new QVBoxLayout(_sidebarArea)),
        _expend(new UniversalTitleBarButton(titleBar()))
    {
        _sidebarLayout->setSpacing(0);
        _sidebarLayout->setContentsMargins(0, 0, 0, 0);
        _sidebarLayout->addWidget(_sidebar);
        _sidebarLayout->addWidget(_bottomFixedArea);
        _globalLayout->removeWidget(_sidebar);
        _globalLayout->insertWidget(0, _sidebarArea);
        _bottomFixedArea->setFixedHeight(64);
        _sidebar->setMinimumWidth(50);
        _sidebarArea->setFixedWidth(50);
        _bottomFixedArea->setMinimumWidth(50);
        _bottomFixedArea->setFixedWidth(50);
        connect(_sidebar->_expandAndRetractAni, &QVariantAnimation::valueChanged, this, &StackWindow::expandOrRetract);

        _titleBar->layout()->setContentsMargins(2, 0, 0, 0);
        _titleBar->resetTitleBtnFixedSize({48, 45});
        _expend->setIcon(iconId(IconPark::HamburgerButton));
        _expend->setFixedSize(45, 45);
        _expend->setRadius(5);
        _expend->setCheckable(true);
        _titleBar->insertWidget(0, _expend);
        connect(_expend, &UniversalTitleBarButton::toggled, _sidebar, &Sidebar::autoExpandRetract);
    }

    void StackWindow::addSidebarOption(SidebarButton *btn)
    {
        _sidebar->addOption(btn);
    }

    void StackWindow::setBottomFixedAreaHeight(int height)
    {
        _bottomFixedArea->setFixedHeight(height);
    }

    QWidget *StackWindow::bottomFixedArea()
    {
        return _bottomFixedArea;
    }

    QPushButton *StackWindow::expendButton()
    {
        return _expend;
    }

    void StackWindow::expandOrRetract(const QVariant &value)
    {
        int w = value.toInt();
        _sidebarArea->setFixedWidth(w);
        _bottomFixedArea->setFixedWidth(w);
    }
} // fancy
