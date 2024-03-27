#include "mainarea.h"
#include "titlebar.h"

CustomerAreaWidget::CustomerAreaWidget(QWidget *parent)
    :QWidget{parent},
    shadow{new QGraphicsDropShadowEffect(this)}
{
}

MainArea::MainArea(QWidget *parent)
    : QWidget{parent},
    vertical_layout{new QVBoxLayout(this)},
    title_bar{new TitleBar(this)},
    customer_area_widget{new CustomerAreaWidget(this)},
    shadow{new QGraphicsDropShadowEffect(this)}
{
    this->vertical_layout->setContentsMargins(0, 0, 0, 0);
    this->vertical_layout->setSpacing(0);

    this->vertical_layout->addWidget(this->title_bar);
    this->vertical_layout->addWidget(this->customer_area_widget);

    this->shadow->setBlurRadius(35);
    this->shadow->setColor(QColor(43, 45, 48, 150));
    this->shadow->setOffset(0, 5);
    this->setGraphicsEffect(this->shadow);
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
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setBrush(QColor(240, 240, 240));
    if (!(this->title_bar->isMax and this->window()->isMaximized()))
        painter.drawRoundedRect(this->rect(), 10, 10);
    else if(this->title_bar->isMax || this->window()->isMaximized())
        painter.drawRect(this->rect());
}
