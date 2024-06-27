#include "titlebarbutton.h"

TitleBarButton::TitleBarButton(QWidget *parent)
    : QPushButton{parent}
{
    this->_mouseState = MouseState::NORMAL;
    this->_radius = 0;
    this->_iconLabel = new QLabel(this);
    this->_isClearBeforeNewPaint = false;
    this->setFixedSize(46, 32);
    this->_normalBrush.setColor(Qt::GlobalColor::transparent);
    this->_hoverBrush.setColor(QColor::fromRgbF((float)0.51, (float)0.51, (float)0.51, (float)0.1));
    this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05)); // 浅色背景点击时颜色减淡，声色背景颜色加深
    this->_pressLeaveBrush = this->_normalBrush;

    this->_normalBrush.setStyle(Qt::BrushStyle::SolidPattern);
    this->_hoverBrush.setStyle(Qt::BrushStyle::SolidPattern);
    this->_pressHoverBrush.setStyle(Qt::BrushStyle::SolidPattern);
    this->_pressLeaveBrush.setStyle(Qt::BrushStyle::SolidPattern);

    int m = this->width() > this->height() ? this->height() : this->width();
    this->_iconLabel->resize(m / 2, m / 2);
    this->_iconLabel->move((this->width() - this->_iconLabel->width()) / 2, (this->height() - this->_iconLabel->height()) / 2);
    this->_iconLabel->setMargin(1);
    this->_iconLabel->setScaledContents(true);

    connect(Theme::themeObject(), &Theme::themeChange, this, &TitleBarButton::onThemeChange);
}

F_DEFINITION_ENTEREVENT(TitleBarButton)
{
    this->setMouseState(MouseState::HOVER);
    QPushButton::enterEvent(event);
}

F_DEFINITION_LEAVEEVENT(TitleBarButton){
    this->setMouseState(MouseState::NORMAL);
    QPushButton::leaveEvent(event);
}

F_DEFINITION_MOUSEPRESSEVENT(TitleBarButton)
{
    this->setMouseState(MouseState::PRESSEDHOVER);
    QPushButton::mousePressEvent(event);
}

F_DEFINITION_MOUSERELEASEEVENT(TitleBarButton)
{
    if (this->rect().contains(event->pos()))
        this->setMouseState(MouseState::HOVER);
    else
        this->setMouseState(MouseState::NORMAL);
    QPushButton::mouseReleaseEvent(event);
}

F_DEFINITION_MOUSEMOVEEVENT(TitleBarButton)
{
    // 不开启鼠标追踪只有左键按下才能触发
    if (this->rect().contains(event->pos()))
        this->setMouseState(MouseState::PRESSEDHOVER);
    else
        this->setMouseState(MouseState::PRESSEDLEAVE);
    QPushButton::mouseMoveEvent(event);
}

void TitleBarButton::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(this->drawBrush());
    painter.setPen(Qt::PenStyle::NoPen);
    if (this->_isClearBeforeNewPaint && (this->_mouseState == MouseState::NORMAL || this->_mouseState == MouseState::PRESSEDLEAVE)) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
    } painter.drawRoundedRect(this->rect(), this->_radius, this->_radius);
}

bool TitleBarButton::isPressed()
{
    return this->_mouseState == MouseState::PRESSEDHOVER || this->_mouseState == MouseState::PRESSEDLEAVE;
}

bool TitleBarButton::isHover()
{
    return this->_mouseState == MouseState::HOVER || this->_mouseState == MouseState::PRESSEDHOVER;
}

void TitleBarButton::setIcon(const QPixmap &icon)
{
    this->_iconLabel->setPixmap(icon);
    this->_iconLabel->update();
}

void TitleBarButton::update()
{
    this->_iconLabel->update();
    QPushButton::update();
}

const QPen &TitleBarButton::drawPen()
{
    switch (this->_mouseState)
    {
    case MouseState::HOVER:
        return this->_hoverPen;
    case MouseState::NORMAL:
        return this->_normalPen;
    case MouseState::PRESSEDHOVER:
        return this->_pressHoverPen;
    default:
        return this->_pressLeavePen;
    }
}

const QBrush &TitleBarButton::drawBrush()
{
    switch (this->_mouseState)
    {
    case MouseState::HOVER:
        return this->_hoverBrush;
    case MouseState::NORMAL:
        return this->_normalBrush;
    case MouseState::PRESSEDHOVER:
        return this->_pressHoverBrush;
    default:
        return this->_pressLeaveBrush;
    }
}
