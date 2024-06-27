#include "widget.h"
#include "AcrylicBrush/acrylicbrush.h"
#include <QPixmap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 400);
    QPixmap pixmap = QPixmap(":/shoko.png").scaled(400,400, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    this->acrylicBrush = new AcrylicBrush(this,pixmap);
    QPainterPath path;
    path.addEllipse(0, 0, 400, 400);
    this->acrylicBrush->setClipPath(path);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    this->acrylicBrush->paint();
    QWidget::paintEvent(event);
}
