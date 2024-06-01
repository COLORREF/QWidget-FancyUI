#include "mainarea.h"

MainArea::MainArea(QWidget *parent)
    : QWidget{parent},
      vertical_layout{new QVBoxLayout(this)},
      title_bar{new StandardTitleBar(this)},
      customer_area_widget{new QWidget(this)}
{
    this->vertical_layout->setContentsMargins(0, 0, 0, 0);
    this->vertical_layout->setSpacing(0);
    this->vertical_layout->addWidget(this->title_bar);
    this->vertical_layout->addWidget(this->customer_area_widget);
}

QWidget *MainArea::customerAreaWidget()
{
    return this->customer_area_widget;
}

StandardTitleBar *MainArea::titleBar()
{
    return this->title_bar;
}

void MainArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter = QPainter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    painter.setBrush(QColor(240, 240, 240));

    if (!this->window()->isMaximized())
    {
        const_cast<QPen&>((painter.pen())).setWidth(0);
        painter.drawRoundedRect(1,1,width()-2,height()-2,10,10);
    }
    else if (this->window()->isMaximized())
    {
        painter.setPen(Qt::PenStyle::NoPen);
        painter.drawRect(this->rect());
    }
}
