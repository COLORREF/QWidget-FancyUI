//
// Created by TX on 2025/11/19.
//

#include "IconPage.h"

#include <QVBoxLayout>
#include <QIcon>

#include "../ui_general_display_page.h"

#include "ExampleControlCard.h"
#include "Widgets/SelectorBar/SelectorBar.h"
#include "Widgets/SelectorBar/SelectorBarItem.h"
#include "Widgets/TextLabel/TextLabel.h"

IconPage::IconPage(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GeneralDisplayPage)
{
    _ui->setupUi(this);
    _ui->header->setMainText("图标");
    _ui->header->setSubText("namespace fancy");
    auto *contents_layout = new QVBoxLayout(_ui->contents);
    contents_layout->setSpacing(32);

    contents_layout->setContentsMargins(36, 28, 36, 28);
    auto *selectorBar = new fancy::SelectorBar(_ui->contents);
    QFont selectorFont = selectorBar->font();
    selectorFont.setPixelSize(18);
    selectorBar->setFont(selectorFont);
    auto *allItem = selectorBar->addItem(QStringLiteral("所有图标"));
    selectorBar->addItem("IconPark", QIcon(QStringLiteral(":/Logos/iconpark.png")));
    selectorBar->addItem("Ant Design", QIcon(QStringLiteral(":/Logos/ant-design.svg")));
    selectorBar->addItem("Bootstrap", QIcon(QStringLiteral(":/Logos/bootstrap.png")));

    for (int i = 0; i < selectorBar->itemCount(); ++i)
        selectorBar->itemAt(i)->setIconSize(QSize(24, 24));

    selectorBar->setSelectedItem(allItem);
    contents_layout->addWidget(selectorBar);

    contents_layout->addStretch(1);
}

IconPage::~IconPage()
{
    delete _ui;
}

ExampleControlCard * IconPage::makeCardLayout(const QString &introductionText, const QString &objName)
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
