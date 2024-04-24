#include "borderlesswindow.h"
#include "border.h"
#include "mainarea.h"

BorderlessWindow::BorderlessWindow(QWidget *parent)
    : QWidget{parent},
      gridLayout{new QGridLayout(this)},
      left_border{new Border(this, BorderType::left_border)},
      left_top_border{new Border(this, BorderType::left_top_border)},
      right_top_border{new Border(this, BorderType::right_top_border)},
      top_border{new Border(this, BorderType::top_border)},
      bottom_border{new Border(this, BorderType::bottom_border)},
      left_bottom_border{new Border(this, BorderType::left_bottom_border)},
      right_border{new Border(this, BorderType::right_border)},
      right_bottom_border{new Border(this, BorderType::right_bottom_border)},
      main_area{new MainArea(this)}
{
    this->setWindowFlags(this->windowFlags() | Qt::WindowType::FramelessWindowHint);
    this->setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
    this->resize(640, 480);

    this->gridLayout->setSpacing(0);
    this->gridLayout->setContentsMargins(0, 0, 0, 0);
    this->gridLayout->addWidget(this->left_border, 1, 1, 1, 1);
    this->gridLayout->addWidget(this->left_top_border, 0, 1, 1, 1);
    this->gridLayout->addWidget(this->right_top_border, 0, 3, 1, 1);
    this->gridLayout->addWidget(this->top_border, 0, 2, 1, 1);
    this->gridLayout->addWidget(this->main_area, 1, 2, 1, 1);
    this->gridLayout->addWidget(this->bottom_border, 2, 2, 1, 1);
    this->gridLayout->addWidget(this->left_bottom_border, 2, 1, 1, 1);
    this->gridLayout->addWidget(this->right_border, 1, 3, 1, 1);
    this->gridLayout->addWidget(this->right_bottom_border, 2, 3, 1, 1);
}

void BorderlessWindow::showMaximized()
{
    for (Border *border : this->findChildren<Border *>())
        if (!border->isHidden())
            border->hide();
    QWidget::showMaximized();
}

void BorderlessWindow::showNormal()
{
    for (Border *border : this->findChildren<Border *>())
        if (border->isHidden())
            border->show();
    QWidget::showNormal();
}
