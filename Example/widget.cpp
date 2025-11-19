//
// Created by TX on 2025/8/2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Widget.h" resolved

#include "widget.h"

#include <chrono>
#include <QButtonGroup>
#include <QDesktopServices>
#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMetaEnum>
#include <QMouseEvent>
#include <QPushButton>
#include <QToolButton>

#include "Defs.h"
#include "FlowLayout.h"
#include "Icon.h"
#include "IconEngine.h"
#include "IconEnums.h"
#include "IconPage.h"
#include "MaximizeButton.h"
#include "Palette.h"
#include "PalettePage.h"
#include "Sidebar.h"
#include "SidebarButton.h"
#include "SvgParsing.h"
#include "SvgWidget.h"
#include "SystemAccessor.h"
#include "ThemeModeController.h"
#include "TitleBar.h"
#include "Main_Program_Page_Code/BasicInput.h"
#include "Main_Program_Page_Code/Home.h"


Widget::Widget(QWidget *parent) :
    StackWindow(parent),
    _homePage(new HomePage(this)),
    _basicInputPage(new BasicInput(this)),
    _iconPage(new IconPage(this)),
    _palettePage(new PalettePage(this))
{
    resize(1200, 740);
    moveToCenter();
    setUpTitleBar();
    setUpSidebar();

    addWidget(_homePage);
    addWidget(_basicInputPage);
    addWidget(_iconPage);
    addWidget(_palettePage);
}

void Widget::setUpSidebar()
{
    addSidebarOption(new fancy::SidebarButton(fancy::IconPark::Home, "主页", sidebar()));
    addSidebarOption(new fancy::SidebarButton(fancy::IconPark::CheckCorrect, "基础控件", sidebar()));
    addSidebarOption(new fancy::SidebarButton(fancy::IconPark::GamePs, "图标", sidebar()));
    addSidebarOption(new fancy::SidebarButton(fancy::IconPark::Platte, "调色板", sidebar()));
    auto *bottomArea = bottomFixedArea();
    auto *layout = new QVBoxLayout(bottomArea);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    auto *bottomSidebar = new fancy::Sidebar(bottomArea);
    layout->addWidget(bottomSidebar);
    auto *settings = new fancy::SidebarButton(fancy::BootstrapIcons::Gear, "设置", bottomArea);
    sidebar()->addOptionToGroup(settings);
    bottomSidebar->addOptionToLayout(settings);
    connect(expendButton(), &QPushButton::toggled, bottomSidebar, &fancy::Sidebar::autoExpandRetract);
}

void Widget::setUpTitleBar()
{
    auto *titlebar = qobject_cast<fancy::TitleBar *>(titleBar());
    auto *themeBtn = new fancy::UniversalTitleBarButton(titleBar());
    themeBtn->setFixedSize(titlebar->titleBtnFixedSize());
    themeBtn->setAutoEnlargeIcon(true);
    titlebar->insertWidget(4, themeBtn);
    auto &themeController = fancy::ThemeModeController::controller();
    auto updateIcon = [themeBtn, &themeController] { themeBtn->setIcon(fancy::iconId(themeController.isAppLight() ? fancy::IconPark::Sleep : fancy::IconPark::Sunrise)); };
    updateIcon();
    connect(themeBtn, &fancy::UniversalTitleBarButton::clicked, &themeController, &fancy::ThemeModeController::toggleAppTheme);
    connect(&fancy::Palette::palette(), &fancy::Palette::appThemeChange, themeBtn, updateIcon);
}
