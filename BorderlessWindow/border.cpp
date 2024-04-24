#include "border.h"

Border::Border(QWidget *parent, BorderType bordertype)
    : QWidget{parent},
      border_type{bordertype}
{
    this->setMouseTracking(true);

    if (this->border_type == BorderType::top_border || this->border_type == BorderType::bottom_border)
        this->setFixedHeight(this->border_width);
    else if (this->border_type == BorderType::left_border || this->border_type == BorderType::right_border)
        this->setFixedWidth(this->border_width);
    else
        this->setFixedSize(this->border_width, this->border_width);
}

void Border::mousePressEvent(QMouseEvent *event)
{
    if (this->window()->isMaximized())
        return QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        switch (this->border_type)
        {
        case BorderType::top_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::TopEdge);
            break;
        case BorderType::bottom_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::BottomEdge);
            break;
        case BorderType::left_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::LeftEdge);
            break;
        case BorderType::right_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::RightEdge);
            break;
        case BorderType::left_top_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::LeftEdge | Qt::Edge::TopEdge);
            break;
        case BorderType::right_bottom_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::RightEdge | Qt::Edge::BottomEdge);
            break;
        case BorderType::left_bottom_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::LeftEdge | Qt::Edge::BottomEdge);
            break;
        case BorderType::right_top_border:
            this->window()->windowHandle()->startSystemResize(Qt::Edge::RightEdge | Qt::Edge::TopEdge);
            break;
        }
    }
    return QWidget::mousePressEvent(event);
}

void Border::mouseMoveEvent(QMouseEvent *event)
{
    if (this->window()->isMaximized())
    {
        this->setCursor(Qt::CursorShape::ArrowCursor);
        return QWidget::mouseMoveEvent(event);
    }
    switch (this->border_type)
    {
    case BorderType::top_border:
        this->setCursor(Qt::CursorShape::SizeVerCursor);
        break;
    case BorderType::bottom_border:
        this->setCursor(Qt::CursorShape::SizeVerCursor);
        break;
    case BorderType::left_border:
        this->setCursor(Qt::CursorShape::SizeHorCursor);
        break;
    case BorderType::right_border:
        this->setCursor(Qt::CursorShape::SizeHorCursor);
        break;
    case BorderType::left_top_border:
        this->setCursor(Qt::CursorShape::SizeFDiagCursor);
        break;
    case BorderType::right_bottom_border:
        this->setCursor(Qt::CursorShape::SizeFDiagCursor);
        break;
    case BorderType::left_bottom_border:
        this->setCursor(Qt::CursorShape::SizeBDiagCursor);
        break;
    case BorderType::right_top_border:
        this->setCursor(Qt::CursorShape::SizeBDiagCursor);
        break;
    }
    return QWidget::mouseMoveEvent(event);
}

void Border::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setBrush(QColor(0,0,0,1));
    painter.drawRect(rect());
}
