#include "maximizebutton.h"

MaximizeButton::MaximizeButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(QPixmap(":/icon/maximize_black.svg"));
    connect(this, &MaximizeButton::stateChange, this, &MaximizeButton::onStateChange);

    connect(this,&QPushButton::clicked,this,[this](){
        if (this->window()->isMaximized())
            this->window()->showNormal();
        else
            this->window()->showMaximized();
    });
}

MaximizeButton::State MaximizeButton::maxmizeState()
{
    return this->state;
}

void MaximizeButton::onStateChange(MaximizeButton::State state)
{
    if (state == State::NORMAL)
        this->setIcon(QPixmap(":/icon/maximize_black.svg"));
    else
        this->setIcon(QPixmap(":/icon/restore_black.svg"));
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
void MaximizeButton::enterEvent(QEnterEvent *event)
#else
void MaximizeButton::enterEvent(QEvent *event)
#endif
{
    this->setMouseState(TitleBarButton::MouseState::HOVER);
    TitleBarButton::enterEvent(event);
}

void MaximizeButton::leaveEvent(QEvent *event)
{
    this->setMouseState(TitleBarButton::MouseState::NORMAL);
    TitleBarButton::leaveEvent(event);
}

void MaximizeButton::mousePressEvent(QMouseEvent *event)
{
    this->setMouseState(TitleBarButton::MouseState::PRESSEDHOVER);
    TitleBarButton::mousePressEvent(event);
}

void MaximizeButton::mouseReleaseEvent(QMouseEvent *event)
{
    QRect max_btn_rect(this->pos(), this->size());
    bool isInSelf = max_btn_rect.contains(event->pos());
    if (isInSelf)
        this->setMouseState(TitleBarButton::MouseState::HOVER);
    else
        this->setMouseState(TitleBarButton::MouseState::NORMAL);
    TitleBarButton::mouseReleaseEvent(event);
}

void MaximizeButton::mouseMoveEvent(QMouseEvent *event)
{
    QRect max_btn_rect(this->pos(), this->size());
    bool isInSelf = max_btn_rect.contains(event->pos());
    if (isInSelf)
        this->setMouseState(TitleBarButton::MouseState::PRESSEDHOVER);
    else
        this->setMouseState(TitleBarButton::MouseState::PRESSEDLEAVE);
    TitleBarButton::mouseMoveEvent(event);
}

