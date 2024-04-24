#include "mainarea.h"
#include "titlebar.h"

CustomerAreaWidget::CustomerAreaWidget(QWidget *parent)
    : QWidget{parent}
{
}

MainArea::MainArea(QWidget *parent)
    : QWidget{parent},
      vertical_layout{new QVBoxLayout(this)},
      title_bar{new TitleBar(this)},
      customer_area_widget{new CustomerAreaWidget(this)}
{
    this->vertical_layout->setContentsMargins(0, 0, 0, 0);
    this->vertical_layout->setSpacing(0);

    this->vertical_layout->addWidget(this->title_bar);
    this->vertical_layout->addWidget(this->customer_area_widget);
}

TitleBar *MainArea::titleBar()
{
    return this->title_bar;
}

void MainArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter = QPainter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    painter.setBrush(QColor(240, 240, 240));

    if (!(this->title_bar->isMax and this->window()->isMaximized()))
    {
        const_cast<QPen&>((painter.pen())).setWidth(0);
        painter.drawRoundedRect(1,1,width()-2,height()-2,10,10);
    }
    else if (this->title_bar->isMax or this->window()->isMaximized())
    {
        painter.setPen(Qt::PenStyle::NoPen);
        painter.drawRect(this->rect());
    }
}
