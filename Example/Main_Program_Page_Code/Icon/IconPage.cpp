//
// Created by TX on 2025/11/19.
//

#include "IconPage.h"

#include <QVBoxLayout>
#include <QIcon>

#include "../ui_general_display_page.h"

#include "IconGallery.h"
#include "Widgets/SelectorBar/SelectorBar.h"
#include "Widgets/SelectorBar/SelectorBarItem.h"

IconPage::IconPage(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::GeneralDisplayPage)
{
    _ui->setupUi(this);
    _ui->header->setMainText("图标");
    _ui->header->setSubText("namespace fancy");
    // 网格和详情各自滚动，移除通用页面的外层滚动区，避免嵌套滚动和高度膨胀
    auto *contents = _ui->scrollArea->takeWidget();
    contents->setAutoFillBackground(false);
    _ui->verticalLayout->removeWidget(_ui->scrollArea);
    _ui->verticalLayout->addWidget(contents, 1);
    delete _ui->scrollArea;
    _ui->scrollArea = nullptr;
    auto *contents_layout = new QVBoxLayout(_ui->contents);
    contents_layout->setSpacing(12);
    contents_layout->setContentsMargins(36, 0, 36, 28);
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

    auto *gallery = new IconGallery(_ui->contents);
    contents_layout->addWidget(gallery, 1);
    connect(selectorBar, &fancy::SelectorBar::selectionChanged, gallery, [selectorBar, gallery](fancy::SelectorBarItem *item) {
        if (item == selectorBar->itemAt(1))
            gallery->setLibrary(fancy::IconStyle::IconPark);
        else if (item == selectorBar->itemAt(2))
            gallery->setLibrary(fancy::IconStyle::AntDesignIcons);
        else if (item == selectorBar->itemAt(3))
            gallery->setLibrary(fancy::IconStyle::BootstrapIcons);
        else
            gallery->setLibrary(std::nullopt);
    });
}

IconPage::~IconPage()
{
    delete _ui;
}
