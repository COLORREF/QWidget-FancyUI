#include "titlebarbutton.h"

TitleBarButton::TitleBarButton(QWidget *parent)
    : QPushButton{parent},
      icon_label{new QLabel(this)}
{
    this->setFixedSize(46, 32);
    this->normal_brush.setColor(Qt::GlobalColor::transparent);
    this->hover_brush.setColor(QColor::fromRgbF((float)0.51, (float)0.51, (float)0.51, (float)0.1));
    this->press_hover_brush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05)); // 浅色背景点击时颜色减淡，声色背景颜色加深
    this->press_leave_brush = this->normal_brush;

    this->normal_brush.setStyle(Qt::BrushStyle::SolidPattern);
    this->hover_brush.setStyle(Qt::BrushStyle::SolidPattern);
    this->press_hover_brush.setStyle(Qt::BrushStyle::SolidPattern);
    this->press_leave_brush.setStyle(Qt::BrushStyle::SolidPattern);

    this->icon_label->move(15, 9);
    this->icon_label->resize(16, 16);
    this->icon_label->setMargin(1);
    this->icon_label->setScaledContents(true);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void TitleBarButton::enterEvent(QEnterEvent *event)
#else
void TitleBarButton::enterEvent(QEvent *event)
#endif
{
    this->setMouseState(MouseState::HOVER);
    QPushButton::enterEvent(event);
}

void TitleBarButton::leaveEvent(QEvent *event)
{
    this->setMouseState(MouseState::NORMAL);
    QPushButton::leaveEvent(event);
}

void TitleBarButton::mousePressEvent(QMouseEvent *event)
{
    this->setMouseState(MouseState::PRESSEDHOVER);
    QPushButton::mousePressEvent(event);
}

void TitleBarButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->rect().contains(event->pos()))
        this->setMouseState(MouseState::HOVER);
    else
        this->setMouseState(MouseState::NORMAL);
    QPushButton::mouseReleaseEvent(event);
}

void TitleBarButton::mouseMoveEvent(QMouseEvent *event)
{
    // 不开启鼠标追踪只有左键按下才能触发
    if (this->rect().contains(event->pos()))
        this->setMouseState(MouseState::PRESSEDHOVER);
    else
        this->setMouseState(MouseState::PRESSEDLEAVE);
    QPushButton::mouseMoveEvent(event);
}

void TitleBarButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(this->drawBrush());
    painter.setPen(Qt::PenStyle::NoPen);
    painter.drawRect(this->rect());
}

void TitleBarButton::setMouseState(MouseState state)
{
    if (this->mouse_state != state)
        this->mouse_state = state;
    else
        return;
    emit this->mouseStateChange(this->mouse_state);
    this->icon_label->update();
    this->update();
}

bool TitleBarButton::isPressed()
{
    return this->mouse_state == MouseState::PRESSEDHOVER || this->mouse_state == MouseState::PRESSEDLEAVE;
}

bool TitleBarButton::isHover()
{
    return this->mouse_state == MouseState::HOVER || this->mouse_state == MouseState::PRESSEDHOVER;
}

void TitleBarButton::setIcon(const QPixmap &icon)
{
    this->icon_label->setPixmap(icon);
    this->icon_label->update();
    this->update();
}

TitleBarButton::MouseState TitleBarButton::mouseState()
{
    return this->mouse_state;
}

void TitleBarButton::update()
{
    this->icon_label->update();
    QPushButton::update();
}

const QPen &TitleBarButton::drawPen()
{
    switch (this->mouse_state)
    {
    case MouseState::HOVER:
        return this->hover_pen;
    case MouseState::NORMAL:
        return this->normal_pen;
    case MouseState::PRESSEDHOVER:
        return this->press_hover_pen;
    default:
        return this->press_leave_pen;
    }
}

const QBrush &TitleBarButton::drawBrush()
{
    switch (this->mouse_state)
    {
    case MouseState::HOVER:
        return this->hover_brush;
    case MouseState::NORMAL:
        return this->normal_brush;
    case MouseState::PRESSEDHOVER:
        return this->press_hover_brush;
    default:
        return this->press_leave_brush;
    }
}
