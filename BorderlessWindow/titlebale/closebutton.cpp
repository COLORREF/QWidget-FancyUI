#include "closebutton.h"

CloseButton::CloseButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(QPixmap(":/icon/close_black.svg"));
    this->hover_brush.setColor(QColor(196, 43, 28));
    this->press_hover_brush.setColor(QColor(200, 64, 49));
    this->connect(this, &CloseButton::clicked, this->window(), &QWidget::close);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void CloseButton::enterEvent(QEnterEvent *event)
#else
void CloseButton::enterEvent(QEvent *event)
#endif
{
    this->setIcon(QPixmap(":/icon/close_white.svg"));
    return TitleBarButton::enterEvent(event);
}

void CloseButton::leaveEvent(QEvent *event)
{
    this->setIcon(QPixmap(":/icon/close_black.svg"));
    return TitleBarButton::leaveEvent(event);
}

void CloseButton::mouseMoveEvent(QMouseEvent *event)
{
    if (this->rect().contains(event->pos()))
        this->setIcon(QPixmap(":/icon/close_white.svg"));
    else
        this->setIcon(QPixmap(":/icon/close_black.svg"));
    TitleBarButton::mouseMoveEvent(event);
}


void CloseButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(this->drawBrush());
    painter.setPen(Qt::PenStyle::NoPen);

    if(this->window()->isMaximized())
        painter.drawRect(this->rect());
    else
    {
        QPainterPath path;
        path.moveTo(this->rect().topLeft().x(),this->rect().topLeft().y()+1);
        path.lineTo(this->rect().topRight().x() - 10, this->rect().topRight().y()+1);
        path.arcTo(this->rect().topRight().x() - 20, this->rect().topRight().y()+1, 20, 20, 90, -90);
        path.lineTo(this->rect().bottomRight());
        path.lineTo(this->rect().bottomLeft());
        path.lineTo(this->rect().topLeft());
        path.closeSubpath();
        painter.drawPath(path);
    }

}
