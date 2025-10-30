//
// Created by TX on 2025/8/2.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Widget.h" resolved

#include "widget.h"
#include "ui_basicInput.h"
#include "ui_home.h"

#include <chrono>
#include <QButtonGroup>
#include <QComboBox>
#include <QDesktopServices>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include  <QPushButton>
#include <QSvgWidget>
#include <QToolButton>
#include "CheckBox.h"
#include "FlowLayout.h"
#include "IconEngine.h"
#include "IconEnums.h"
#include "RadioButton.h"
#include "RadioButtonStyle.h"
#include "SvgParsing.hpp"
#include "SystemAccessor.h"
#include "WindowController.h"

#include "CloseButton.h"
#include "ControlState.h"
#include "ControlState.h"
#include "Defs.hpp"
#include "ExampleNavigationCard.h"
#include "Icon.h"
#include "IntroductionCard.h"
#include "MaximizeButton.h"
#include "MinimizeButton.h"
#include "Palette.h"
#include "PushButton.h"
#include "PushButtonStyle.h"
#include "RippleButton.h"
#include "ScrollBar.h"
#include "ScrollBarStyle.h"
#include "Sidebar.h"
#include "SidebarButton.h"
#include "Slider.h"
#include "SliderIndicator.h"
#include "SvgWidget.h"
#include "TextLabelStyle.h"
#include "ThemeButton.h"
#include "ThemeModeController.h"
#include "TitleBar.h"
#include "TransparentButton.h"


Widget::Widget(QWidget *parent) :
    StackWindow(parent),
    _home(new QWidget(this)),
    _basicInput(new QWidget(this)),
    _uiHome(new Ui::Home),
    _uiBasicInput(new Ui::BasicInput)
{
    resize(1200, 740);
    moveToCenter();
    setUpTitleBar();
    setUpHome();
    setUpBasicInput();
    setUpSidebar();
}


Widget::~Widget()
{
    delete _uiHome;
    delete _uiBasicInput;
}

void Widget::setUpHome()
{
    _uiHome->setupUi(_home);
    addWidget(_home);

    _uiHome->homeImage->setRadius(5);
    _uiHome->homeImage->setFillBackground(true);
    _uiHome->homeImage->setFillColor(ThemeModeController::controller().isAppLight() ? QColor(208, 217, 228) : QColor(2, 11, 32));
    _uiHome->homeImage->setPixmap(QPixmap(":/home.png"));

    auto github = new IntroductionCard(_uiHome->homeImage, _uiHome->homeImage);
    github->setBlurRadius(30);
    github->setIcon(iconId(BootstrapIcons::Github));
    github->setMainText("FancyUI On Github");
    github->setSubText("Explore the FancyUI source code\nand repository.");
    github->setUrl(QUrl(R"(https://github.com/COLORREF/QWidget-FancyUI)"));
    github->move(20, 260);

    auto bilibili = new IntroductionCard(_uiHome->homeImage, _uiHome->homeImage);
    bilibili->setBlurRadius(30);
    bilibili->setIcon(iconId(AntDesignIcons::Bilibili));
    bilibili->setMainText("Developer's homepage");
    bilibili->setSubText("Contact the developer \nor provide your feedback.\nLook forward to your attention.");
    bilibili->setUrl(QUrl(R"(https://m.bilibili.com/space/1843315943)"));
    bilibili->move(280, 260);

    auto onPixmapUpdate = [github, bilibili] {
        github->blur();
        bilibili->blur();
    };
    auto onThemeChanged = [this, onPixmapUpdate] {
        _uiHome->homeImage->setFillColor(ThemeModeController::controller().isAppLight() ? QColor(208, 217, 228) : QColor(2, 11, 32));
        onPixmapUpdate();
    };
    connect(_uiHome->homeImage, &CenteredImageWidget::SmoothPixmapUpdate, this, onPixmapUpdate);
    connect(_uiHome->homeImage, &CenteredImageWidget::FastTPixmapUpdate, this, onPixmapUpdate);
    connect(&Palette::palette(), &Palette::appThemeChange, this, onThemeChanged);

    auto button = new ExampleNavigationCard(_uiHome->homeImage);
    button->setIcon(QPixmap(":/Button.png"));
    button->setMainText("Button");
    button->setSubText("A control that responds to user input and\nraises a Click event.");

    auto acrylicBrush = new ExampleNavigationCard(_uiHome->homeImage);
    acrylicBrush->setIcon(QPixmap(":/Acrylic.png"));
    acrylicBrush->setMainText("AcrylicBrush");
    acrylicBrush->setSubText("A translucent material recommended for\npanel backgrounds.");

    auto hyperlinkButton = new ExampleNavigationCard(_uiHome->homeImage);
    hyperlinkButton->setIcon(QPixmap(":/HyperlinkButton.png"));
    hyperlinkButton->setMainText("HyperlinkButton");
    hyperlinkButton->setSubText("A button that appears as hyperlink text,\nand can navigate to a URl or handle a\nClick event.");

    auto flowLayout = new FlowLayout(_uiHome->exhibitionArea);
    flowLayout->addWidget(button);
    flowLayout->addWidget(acrylicBrush);
    flowLayout->addWidget(hyperlinkButton);
}

void Widget::setUpBasicInput()
{
    _uiBasicInput->setupUi(_basicInput);
    addWidget(_basicInput);
}

void Widget::setUpSidebar()
{
    addSidebarOption(new SidebarButton(IconPark::Home, "主页", sidebar()));
    addSidebarOption(new SidebarButton(IconPark::Components, "基础控件", sidebar()));
    addSidebarOption(new SidebarButton(IconPark::GamePs, "图标", sidebar()));

    auto *bottomArea = bottomFixedArea();
    auto *layout = new QVBoxLayout(bottomArea);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    auto *bottomSidebar = new Sidebar(bottomArea);
    layout->addWidget(bottomSidebar);
    auto *settings = new SidebarButton(IconPark::Config, "设置", bottomArea);
    sidebar()->addOptionToGroup(settings);
    bottomSidebar->addOptionToLayout(settings);
    connect(expendButton(), &QPushButton::toggled, bottomSidebar, &Sidebar::autoExpandRetract);
}

void Widget::setUpTitleBar()
{
    auto *titlebar = qobject_cast<TitleBar *>(titleBar());
    auto *themeBtn = new UniversalTitleBarButton(titleBar());
    themeBtn->setFixedSize(titlebar->titleBtnFixedSize());
    themeBtn->setAutoEnlargeIcon(true);
    titlebar->insertWidget(4, themeBtn);
    auto &themeController = ThemeModeController::controller();
    auto updateIcon = [themeBtn, &themeController] { themeBtn->setIcon(iconId(themeController.isAppLight() ? IconPark::Sleep : IconPark::Sunrise)); };
    updateIcon();
    connect(themeBtn, &UniversalTitleBarButton::clicked, &themeController, &ThemeModeController::toggleAppTheme);
    connect(&Palette::palette(), &Palette::appThemeChange, themeBtn, updateIcon);
}
