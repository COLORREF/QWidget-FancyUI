#include "widget.h"
#include "acrylicmaterial.h"
#include <QPixmap>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 400);
    QPixmap pixmap = QPixmap(":/shoko.png").scaled(400,400, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    this->acrylicMaterial = new AcrylicMaterial(this,pixmap);
    QPainterPath path;
    path.addEllipse(0, 0, 400, 400);
    this->acrylicMaterial->setClipPath(path);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *event)
{
    this->acrylicMaterial->paint();
    QWidget::paintEvent(event);
}
