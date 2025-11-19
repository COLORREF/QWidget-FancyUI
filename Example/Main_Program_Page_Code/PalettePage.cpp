//
// Created by TX on 2025/11/19.
//

#include "PalettePage.h"

#include <QDesktopServices>
#include <ui_general_display_page.h>

#include "DerivativeColorSelector.h"
#include "ExampleControlCard.h"
#include "ExampleDefs.h"
#include "PushButton.h"
#include "TextLabel.h"
#include "ThemeModeController.h"
#include "TransparentButton.h"

PalettePage::PalettePage(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GeneralDisplayPage)
{
    _ui->setupUi(this);
    _ui->header->setMainText(R"(调色板)");
    _ui->header->setSubText("namespace fancy");
    auto *contents_layout = new QVBoxLayout(_ui->contents);
    contents_layout->setSpacing(32);

    connect(_ui->header->_hyperlinkDocument, &fancy::PushButton::clicked, this, std::bind(QDesktopServices::openUrl, QUrl(R"(https://2x.ant.design/docs/spec/colors-cn#%E8%89%B2%E6%9D%BF)")));

    // AntDesign2.xPalettes
    {
        auto *antDesignPalettes_Card = makeCardLayout("基于 Ant Design 2.x 调色板生成算法生成的衍生色", "AntDesign2.xPalettes");
        auto *content = antDesignPalettes_Card->controlAreaWidget();
        auto *content_layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *wrap_widget = new QWidget(content);
        auto *wrap_layout = new QVBoxLayout(wrap_widget);
        wrap_layout->setContentsMargins(0, 0, 0, 0);
        wrap_layout->setSpacing(5);
        auto *derivativeColorSelector_red = new DerivativeColorSelector(wrap_widget, QColor(240, 65, 52), "Red");
        auto *derivativeColorSelector_green = new DerivativeColorSelector(wrap_widget, QColor(0, 168, 84), "Green");
        auto *derivativeColorSelector_blue = new DerivativeColorSelector(wrap_widget, QColor(16, 142, 233), "Blue");
        auto *derivativeColorSelector_pink = new DerivativeColorSelector(wrap_widget, QColor(245, 49, 127), "Pink");
        auto *derivativeColorSelector_orange = new DerivativeColorSelector(wrap_widget, QColor(245, 106, 0), "Orange");
        auto *derivativeColorSelector_purple = new DerivativeColorSelector(wrap_widget, QColor(114, 101, 230), "Purple");
        auto *derivativeColorSelector_yellow = new DerivativeColorSelector(wrap_widget, QColor(255, 191, 0), "Yellow");
        auto *derivativeColorSelector_cyan = new DerivativeColorSelector(wrap_widget, QColor(0, 162, 174), "Cyan");
        auto *derivativeColorSelector_gray = new DerivativeColorSelector(wrap_widget, QColor(191, 191, 191), "Gray");
        wrap_layout->addWidget(derivativeColorSelector_red, 0);
        wrap_layout->addWidget(derivativeColorSelector_green, 0);
        wrap_layout->addWidget(derivativeColorSelector_blue, 0);
        wrap_layout->addWidget(derivativeColorSelector_pink, 0);
        wrap_layout->addWidget(derivativeColorSelector_orange, 0);
        wrap_layout->addWidget(derivativeColorSelector_purple, 0);
        wrap_layout->addWidget(derivativeColorSelector_yellow, 0);
        wrap_layout->addWidget(derivativeColorSelector_cyan, 0);
        wrap_layout->addWidget(derivativeColorSelector_gray, 0);
        content_layout->addWidget(wrap_widget, 0);
        content_layout->addStretch(1);
    }
    contents_layout->addStretch(1);

    for (const auto &card: _controlCards)
        card->setHtmlCode(loadExampleCode(card->objectName(), fancy::ThemeModeController::controller().appTheme()));

    connect(&fancy::ThemeModeController::controller(),
            &fancy::ThemeModeController::appThemeChange,
            this,
            [this](fancy::Theme theme) {
                for (const auto &card: _controlCards)
                    card->setHtmlCode(loadExampleCode(card->objectName(), theme));
            }
    );
}

PalettePage::~PalettePage()
{
    delete _ui;
}

ExampleControlCard *PalettePage::makeCardLayout(const QString &introductionText, const QString &objName)
{
    auto *peripheral = new QWidget(_ui->contents);
    auto introductionLabel = new fancy::TextLabel(introductionText, peripheral);
    QFont font(introductionLabel->font());
    font.setPointSizeF(10.5);
    introductionLabel->setFont(font);
    auto controlCard = new ExampleControlCard(peripheral);
    controlCard->setObjectName(objName);
    _controlCards.append(controlCard);
    controlCard->setTitleText("源代码");
    auto *control_content_layout = new QHBoxLayout(controlCard->controlAreaWidget());
    control_content_layout->setContentsMargins(16, 12, 12, 16);

    auto *layout = new QVBoxLayout(peripheral);
    layout->setSpacing(12);
    layout->setContentsMargins(36, 0, 36, 0);
    layout->addWidget(introductionLabel);
    layout->addWidget(controlCard);

    _ui->contents->layout()->addWidget(peripheral);
    return controlCard;
}
