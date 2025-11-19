//
// Created by TX on 2025/11/17.
//

#include "ExamplePageHeader.h"

#include <QDesktopServices>
#include <QVBoxLayout>

#include "Palette.h"
#include "PushButton.h"
#include "TextLabel.h"
#include "ThemeModeController.h"

ExamplePageHeader::ExamplePageHeader(QWidget *parent) :
    QWidget(parent),
    _mainTitle(new fancy::TextLabel(this)),
    _subTitle(new QLabel(this)),
    _hyperlinkDocument(new fancy::PushButton(this)),
    _hyperlinkGithub(new fancy::PushButton(this)),
    _theme(new fancy::PushButton(this)),
    _supportAuthor(new fancy::PushButton(this))
{
    setFixedHeight(155);

    QFont font = _mainTitle->font();
    font.setPointSizeF(24);

    _mainTitle->setText("Main Text");
    _mainTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    _mainTitle->setFont(font);

    _subTitle->setText("Sub Text");
    _subTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    auto palette = _subTitle->palette();
    palette.setColor(QPalette::WindowText, fancy::ThemeModeController::controller().isAppLight() ? QColor(96, 96, 96) : QColor(206, 206, 206));
    connect(&fancy::Palette::palette(),
            &fancy::Palette::appThemeChange,
            this,
            [this] {
                auto palette = _subTitle->palette();
                palette.setColor(QPalette::WindowText, fancy::ThemeModeController::controller().isAppLight() ? QColor(96, 96, 96) : QColor(206, 206, 206));
                _subTitle->setPalette(palette);
            }
    );
    _subTitle->setPalette(palette);

    _hyperlinkDocument->setText("文档");
    _hyperlinkDocument->setIcon(fancy::BootstrapIcons::FileEarmark);
    _hyperlinkDocument->setIconSize({20, 20});
    _hyperlinkDocument->setFixedSize(75, 36);

    _hyperlinkGithub->setText("源代码");
    _hyperlinkGithub->setIcon(fancy::BootstrapIcons::Github);
    _hyperlinkGithub->setIconSize({20, 20});
    _hyperlinkGithub->setFixedSize(85, 36);

    connect(_hyperlinkGithub, &fancy::PushButton::clicked, this, std::bind(QDesktopServices::openUrl, QUrl(R"(https://github.com/COLORREF/QWidget-FancyUI)")));

    _theme->setFixedSize(38, 34);
    auto updateIcon = [this] { _theme->setIcon(fancy::ThemeModeController::controller().isAppLight() ? fancy::BootstrapIcons::MoonStars : fancy::BootstrapIcons::BrightnessHigh); };
    updateIcon();
    connect(_theme, &fancy::PushButton::clicked, &fancy::ThemeModeController::controller(), &fancy::ThemeModeController::toggleAppTheme);
    connect(&fancy::Palette::palette(), &fancy::Palette::appThemeChange, _theme, updateIcon);

    _supportAuthor->setFixedSize(38, 34);
    _supportAuthor->setIcon(fancy::BootstrapIcons::Heart);
    connect(_supportAuthor, &fancy::PushButton::clicked, this, std::bind(QDesktopServices::openUrl, QUrl(R"(https://m.bilibili.com/space/1843315943)")));

    auto *vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(36, 28, 36, 16);
    vLayout->setSpacing(12);

    auto* wrap = new QWidget(this);
    auto *hLayout = new QHBoxLayout(wrap);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(5);
    hLayout->addWidget(_hyperlinkDocument);
    hLayout->addWidget(_hyperlinkGithub);
    hLayout->addStretch(1);
    hLayout->addWidget(_theme);
    hLayout->addWidget(_supportAuthor);


    vLayout->addWidget(_mainTitle);
    vLayout->addWidget(_subTitle);
    vLayout->addWidget(wrap);
}

void ExamplePageHeader::setMainText(const QString &text)
{
    _mainTitle->setText(text);
}

void ExamplePageHeader::setSubText(const QString &text)
{
    _subTitle->setText(text);
}
