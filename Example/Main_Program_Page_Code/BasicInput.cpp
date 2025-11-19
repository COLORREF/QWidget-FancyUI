//
// Created by TX on 2025/11/18.
//

#include "BasicInput.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMetaEnum>

#include "CheckBox.h"
#include "ExampleControlCard.h"
#include "ExampleDefs.h"
#include "Icon.h"
#include "PushButton.h"
#include "RadioButton.h"
#include "RippleButton.h"
#include "ScrollBar.h"
#include "Slider.h"
#include "TextLabel.h"
#include "ThemeButton.h"
#include "ThemeModeController.h"
#include "TransparentButton.h"
#include "ui_general_display_page.h"

BasicInput::BasicInput(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GeneralDisplayPage)
{
    _ui->setupUi(this);
    _ui->header->setMainText("基础控件");
    _ui->header->setSubText("namespace fancy");
    auto *contents_layout = new QVBoxLayout(_ui->contents);
    contents_layout->setSpacing(32);

    connect(_ui->header->_hyperlinkDocument, &fancy::PushButton::clicked, this, []() { qDebug() << "暂时还没有文档 （；´д｀）ゞ "; });

    // PushButton
    {
        auto *pushButton_Card = makeCardLayout("简单的文本按钮", "PushButton");
        auto *content = pushButton_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *pushButton = new fancy::PushButton("标准按钮", content);
        pushButton->setFixedSize(120, 40);
        layout->addWidget(pushButton, 0);
        layout->addStretch(1);
    }

    // ToggleButton
    {
        auto *toggleButton_Card = makeCardLayout("普通的状态按钮", "ToggleButton");
        auto *content = toggleButton_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *toggleButton = new fancy::PushButton("状态按钮", content);
        toggleButton->setFixedSize(120, 40);
        toggleButton->setCheckable(true);
        layout->addWidget(toggleButton, 0);
        layout->addStretch(1);
    }

    // ThemeButton
    {
        auto *themeButton_Card = makeCardLayout("主题色按钮", "ThemeButton");
        auto *content = themeButton_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *themeButton = new fancy::ThemeButton("主题色按钮", content);
        themeButton->setFixedSize(120, 40);
        layout->addWidget(themeButton, 0);
        layout->addStretch(1);
    }

    // TransparentButton
    {
        auto *transparentButton_Card = makeCardLayout("透明按钮", "TransparentButton");
        auto *content = transparentButton_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *transparentButton = new fancy::TransparentButton("透明按钮", content);
        transparentButton->setFixedSize(120, 40);
        layout->addWidget(transparentButton, 0);
        layout->addStretch(1);
    }

    // TransparentButtonWithBorder
    {
        auto *transparentButtonWithBorder_Card = makeCardLayout("带有边框的透明按钮", "TransparentButtonWithBorder");
        auto *content = transparentButtonWithBorder_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *transparentButtonWithBorder = new fancy::TransparentButton("带有边框的透明按钮", content);
        transparentButtonWithBorder->setFixedSize(155, 40);
        transparentButtonWithBorder->setDrawBorder(true);
        layout->addWidget(transparentButtonWithBorder, 0);
        layout->addStretch(1);
    }

    // TransparentToggleButton
    {
        auto *transparentToggleButton_Card = makeCardLayout("透明状态按钮", "TransparentToggleButton");
        auto *content = transparentToggleButton_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *transparentToggleButton = new fancy::TransparentButton("透明状态按钮", content);
        transparentToggleButton->setFixedSize(120, 40);
        transparentToggleButton->setCheckable(true);
        layout->addWidget(transparentToggleButton, 0);
        layout->addStretch(1);
    }

    // RippleButton
    {
        auto *rippleButton_Card = makeCardLayout("涟漪按钮", "RippleButton");
        auto *content = rippleButton_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *rippleButton = new fancy::RippleButton("涟漪按钮", content);
        rippleButton->setFixedSize(120, 40);
        layout->addWidget(rippleButton, 0);
        layout->addStretch(1);
    }

    // ButtonWithIcon
    {
        auto *pushButtonWithIcon_Card = makeCardLayout("带有图标的按钮", "ButtonWithIcon");
        auto *content = pushButtonWithIcon_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *buttonWithIcon_1 = new fancy::RippleButton(content);
        buttonWithIcon_1->setIcon(fancy::AntDesignIcons::Translation);
        buttonWithIcon_1->setIconSize({28, 28});
        buttonWithIcon_1->setFixedSize(40, 40);
        auto *buttonWithIcon_2 = new fancy::PushButton(fancy::BootstrapIcons::PencilSquare, "带有图标的按钮", content);
        buttonWithIcon_2->setIconSize({20, 20});
        buttonWithIcon_2->setFixedSize(160, 40);
        auto *buttonWithIcon_3 = new fancy::PushButton(content);
        buttonWithIcon_3->setIcon(QIcon(":/Slices.png"));
        buttonWithIcon_3->setIconSize(QSize(30, 30));
        buttonWithIcon_3->setFixedSize(40, 40);
        auto *buttonWithIcon_4 = new fancy::TransparentButton(content);
        buttonWithIcon_4->setIcon(fancy::IconPark::WinkingFaceWithOpenEyes);
        buttonWithIcon_4->setIconSize({30, 30});
        buttonWithIcon_4->setFixedSize(40, 40);

        auto *clickedButtonTextLabel = new fancy::TextLabel("点击了按钮: ", content);
        connect(buttonWithIcon_1, &fancy::PushButton::clicked, this, [clickedButtonTextLabel]() { clickedButtonTextLabel->setText("点击了按钮: buttonWithIcon_1"); });
        connect(buttonWithIcon_2, &fancy::PushButton::clicked, this, [clickedButtonTextLabel]() { clickedButtonTextLabel->setText("点击了按钮: buttonWithIcon_2"); });
        connect(buttonWithIcon_3, &fancy::PushButton::clicked, this, [clickedButtonTextLabel]() { clickedButtonTextLabel->setText("点击了按钮: buttonWithIcon_3"); });
        connect(buttonWithIcon_4, &fancy::PushButton::clicked, this, [clickedButtonTextLabel]() { clickedButtonTextLabel->setText("点击了按钮: buttonWithIcon_4"); });

        layout->addWidget(buttonWithIcon_1, 0);
        layout->addWidget(buttonWithIcon_2, 0);
        layout->addWidget(buttonWithIcon_3, 0);
        layout->addWidget(buttonWithIcon_4, 0);
        layout->addStretch(1);
        layout->addWidget(clickedButtonTextLabel, 0);
    }

    //RoundedButton
    {
        auto *roundedButton_Card = makeCardLayout("圆角按钮", "RoundedButton");
        auto *content = roundedButton_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        fancy::PushButton *roundedButton_1 = new fancy::PushButton("圆角按钮", content);
        roundedButton_1->setFixedSize(120, 40);
        roundedButton_1->setRadius(10);

        fancy::ThemeButton *roundedButton_2 = new fancy::ThemeButton(content);
        roundedButton_2->setIcon(fancy::IconPark::Cycle);
        roundedButton_2->setIconSize(QSize(28, 28));
        roundedButton_2->setFixedSize(40, 40);
        roundedButton_2->setRadius(10);

        auto *slider = new fancy::Slider(Qt::Orientation::Horizontal, content);
        slider->setMinimumWidth(150);
        slider->setRange(0, 20);
        slider->setValue(10);
        auto *sliderValueLabel = new fancy::TextLabel("圆角半径: 10 ", content);

        connect(slider,
                &fancy::Slider::valueChanged,
                this,
                [roundedButton_1,roundedButton_2,sliderValueLabel](int value) {
                    roundedButton_1->setRadius(value);
                    roundedButton_2->setRadius(value);
                    sliderValueLabel->setText(QString("圆角半径: %1 ").arg(value));
                }
        );

        layout->addWidget(roundedButton_1, 0);
        layout->addWidget(roundedButton_2, 0);
        layout->addStretch(1);
        layout->addWidget(sliderValueLabel, 0);
        layout->addWidget(slider, 0);
    }

    // Checkbox
    {
        auto *checkBox_Card = makeCardLayout("双态复选框", "Checkbox");
        auto *content = checkBox_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *checkBox = new fancy::CheckBox("双态复选框", content);
        layout->addWidget(checkBox, 0);
        layout->addStretch(1);
    }

    // SmallCheckbox
    {
        auto *checkBox_Card = makeCardLayout("小尺寸的双态复选框", "SmallCheckbox");
        auto *content = checkBox_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *checkBox = new fancy::CheckBox("小尺寸双态复选框", content, true);
        layout->addWidget(checkBox, 0);
        layout->addStretch(1);
    }

    // RadioButton
    {
        auto *radioButton_Card = makeCardLayout("单选按钮", "RadioButton");
        auto *content = radioButton_Card->controlAreaWidget();

        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());

        auto *wrap = new QWidget(this);
        auto *vLayout = new QVBoxLayout(wrap);

        auto *radioButton_1 = new fancy::RadioButton("选项 1", content);
        radioButton_1->setFixedSize(120, 40);
        auto *radioButton_2 = new fancy::RadioButton("选项 2", content);
        radioButton_2->setFixedSize(120, 40);
        auto *radioButton_3 = new fancy::RadioButton("选项 3", content);
        radioButton_3->setFixedSize(120, 40);
        vLayout->addWidget(radioButton_1, 0);
        vLayout->addWidget(radioButton_2, 0);
        vLayout->addWidget(radioButton_3, 0);
        layout->addWidget(wrap);
        layout->addStretch(1);
    }

    // Slider
    {
        auto *slider_Card = makeCardLayout("简单的滑动条", "Slider");
        auto *content = slider_Card->controlAreaWidget();
        auto *layout = qobject_cast<QHBoxLayout *>(content->layout());
        auto *horizontalSlider = new fancy::Slider(Qt::Orientation::Horizontal, content);
        horizontalSlider->setMinimumWidth(200);
        horizontalSlider->setRange(0, 100);
        horizontalSlider->setValue(50);
        auto *verticalSlider = new fancy::Slider(Qt::Orientation::Vertical, content);
        verticalSlider->setMinimumHeight(150);
        verticalSlider->setRange(0, 100);
        verticalSlider->setValue(50);
        connect(horizontalSlider, &fancy::Slider::valueChanged, verticalSlider, &fancy::Slider::setValue);
        connect(verticalSlider, &fancy::Slider::valueChanged, horizontalSlider, &fancy::Slider::setValue);

        auto *valueTextLabel = new fancy::TextLabel("当前值: 50", content);
        connect(horizontalSlider, &fancy::Slider::valueChanged, this, [valueTextLabel](int value) { valueTextLabel->setText(QString("当前值: %1").arg(value)); });

        layout->addWidget(horizontalSlider, 0);
        layout->addWidget(verticalSlider, 0);
        layout->addStretch(1);
        layout->addWidget(valueTextLabel, 0);
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

BasicInput::~BasicInput()
{
    delete _ui;
}

ExampleControlCard *BasicInput::makeCardLayout(const QString &introductionText, const QString &objName)
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
