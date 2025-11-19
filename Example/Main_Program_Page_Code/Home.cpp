//
// Created by TX on 2025/11/18.
//

#include "Home.h"

#include "Defs.h"
#include "ExampleNavigationCard.h"
#include "FlowLayout.h"
#include "IntroductionCard.h"
#include "Palette.h"
#include "ThemeModeController.h"
#include "ui_home.h"

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    _uiHome(new Ui::Home)
{
    _uiHome->setupUi(this);

    _uiHome->homeImage->setRadius(5);
    _uiHome->homeImage->setFillBackground(true);
    _uiHome->homeImage->setFillColor(fancy::ThemeModeController::controller().isAppLight() ? QColor(208, 217, 228) : QColor(2, 11, 32));
    _uiHome->homeImage->setPixmap(QPixmap(":/home.png"));

    auto github = new fancy::IntroductionCard(_uiHome->homeImage, _uiHome->homeImage);
    github->setBlurRadius(30);
    github->setIcon(fancy::iconId(fancy::BootstrapIcons::Github));
    github->setMainText("FancyUI On Github");
    github->setSubText("Explore the FancyUI source code\nand repository.");
    github->setUrl(QUrl(R"(https://github.com/COLORREF/QWidget-FancyUI)"));
    github->move(20, 260);

    auto bilibili = new fancy::IntroductionCard(_uiHome->homeImage, _uiHome->homeImage);
    bilibili->setBlurRadius(30);
    bilibili->setIcon(fancy::iconId(fancy::AntDesignIcons::Bilibili));
    bilibili->setMainText("Developer's homepage");
    bilibili->setSubText("Contact the developer \nor provide your feedback.\nLook forward to your attention.");
    bilibili->setUrl(QUrl(R"(https://m.bilibili.com/space/1843315943)"));
    bilibili->move(280, 260);

    auto onPixmapUpdate = [github, bilibili] {
        github->blur();
        bilibili->blur();
    };
    auto onThemeChanged = [this, onPixmapUpdate] {
        _uiHome->homeImage->setFillColor(fancy::ThemeModeController::controller().isAppLight() ? QColor(208, 217, 228) : QColor(2, 11, 32));
        onPixmapUpdate();
    };
    connect(_uiHome->homeImage, &fancy::CenteredImageWidget::SmoothPixmapUpdate, this, onPixmapUpdate);
    connect(_uiHome->homeImage, &fancy::CenteredImageWidget::FastTPixmapUpdate, this, onPixmapUpdate);
    connect(&fancy::Palette::palette(), &fancy::Palette::appThemeChange, this, onThemeChanged);

    auto button = new fancy::ExampleNavigationCard(_uiHome->homeImage);
    button->setIcon(QPixmap(":/Button.png"));
    button->setMainText("Button");
    button->setSubText("A control that responds to user input and\nraises a Click event.");

    auto acrylicBrush = new fancy::ExampleNavigationCard(_uiHome->homeImage);
    acrylicBrush->setIcon(QPixmap(":/Acrylic.png"));
    acrylicBrush->setMainText("AcrylicBrush");
    acrylicBrush->setSubText("A translucent material recommended for\npanel backgrounds.");

    auto hyperlinkButton = new fancy::ExampleNavigationCard(_uiHome->homeImage);
    hyperlinkButton->setIcon(QPixmap(":/HyperlinkButton.png"));
    hyperlinkButton->setMainText("HyperlinkButton");
    hyperlinkButton->setSubText("A button that appears as hyperlink text,\nand can navigate to a URl or handle a\nClick event.");

    auto flowLayout = new fancy::FlowLayout(_uiHome->exhibitionArea);
    flowLayout->addWidget(button);
    flowLayout->addWidget(acrylicBrush);
    flowLayout->addWidget(hyperlinkButton);
}

HomePage::~HomePage()
{
    delete _uiHome;
}
