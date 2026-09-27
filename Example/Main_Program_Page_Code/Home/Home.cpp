//
// Created by TX on 2025/11/18.
//

#include "Home.h"

#include "ExampleNavigationCard.h"
#include "IntroductionCard.h"
#include "ui_home.h"
#include "Core/Palette/Palette.h"
#include "Core/Theme/ThemeModeController.h"
#include "Layout/FlowLayout.h"
#include "Core/Defs.h"

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    _uiHome(new Ui::Home),
    _githubCard(nullptr),
    _bilibiliCard(nullptr),
    _fastBlurSignalCount(-1)
{
    initializeUi();
    connect(_uiHome->homeImage, &fancy::CenteredImageWidget::FastTPixmapUpdate, this, &HomePage::onFastPixmapUpdate);
    connect(_uiHome->homeImage, &fancy::CenteredImageWidget::SmoothPixmapUpdate, this, &HomePage::updateSmoothBlur);
    connect(&fancy::Palette::palette(), &fancy::Palette::appThemeChange, this, &HomePage::onThemeChanged);
}

void HomePage::initializeUi()
{
    _uiHome->setupUi(this);

    _uiHome->homeImage->setRadius(5);
    _uiHome->homeImage->setFillBackground(true);
    _uiHome->homeImage->setFillColor(
        fancy::ThemeModeController::controller().isAppLight()
            ? QColor(208, 217, 228)
            : QColor(2, 11, 32)
    );
    _uiHome->homeImage->setPixmap(QPixmap(":/home.png"));

    _githubCard = new fancy::IntroductionCard(_uiHome->homeImage, _uiHome->homeImage);
    _githubCard->setBlurRadius(30);
    _githubCard->setIcon(fancy::iconId(fancy::BootstrapIcons::Github));
    _githubCard->setMainText("FancyUI On Github");
    _githubCard->setSubText("Explore the FancyUI source code\nand repository.");
    _githubCard->setUrl(QUrl(R"(https://github.com/COLORREF/QWidget-FancyUI)"));
    _githubCard->move(20, 260);

    _bilibiliCard = new fancy::IntroductionCard(_uiHome->homeImage, _uiHome->homeImage);
    _bilibiliCard->setBlurRadius(30);
    _bilibiliCard->setIcon(fancy::iconId(fancy::AntDesignIcons::Bilibili));
    _bilibiliCard->setMainText("Developer's homepage");
    _bilibiliCard->setSubText("Contact the developer \nor provide your feedback.\nLook forward to your attention.");
    _bilibiliCard->setUrl(QUrl(R"(https://m.bilibili.com/space/1843315943)"));
    _bilibiliCard->move(280, 260);

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

void HomePage::onFastPixmapUpdate()
{
    if (_fastBlurSignalCount < 0 || ++_fastBlurSignalCount >= FastBlurMergeCount)
    {
        // 预览截图的有效 DPR 为 1，减少快速缩放期间的滤波像素数
        const qreal scale = 1.0 / qMax<qreal>(1.0, _uiHome->homeImage->devicePixelRatioF());
        _githubCard->setResolutionScale(scale);
        _bilibiliCard->setResolutionScale(scale);
        _githubCard->blur();
        _bilibiliCard->blur();
        _fastBlurSignalCount = 0;
    }
}

void HomePage::updateSmoothBlur()
{
    _fastBlurSignalCount = -1;
    _githubCard->setResolutionScale(1.0);
    _bilibiliCard->setResolutionScale(1.0);
    _githubCard->blur();
    _bilibiliCard->blur();
}

void HomePage::onThemeChanged()
{
    _uiHome->homeImage->setFillColor(
        fancy::ThemeModeController::controller().isAppLight()
            ? QColor(208, 217, 228)
            : QColor(2, 11, 32)
    );
    updateSmoothBlur();
}

HomePage::~HomePage()
{
    delete _uiHome;
}
