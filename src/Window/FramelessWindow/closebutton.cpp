#include "closebutton.h"

CloseButton::CloseButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(QPixmap(":/icon/close_black.svg"));
    this->_hoverBrush.setColor(QColor(196, 43, 28));
    this->_pressHoverBrush.setColor(QColor(200, 64, 49));
    this->connect(this, &CloseButton::clicked, this->window(), &QWidget::close);
}

void CloseButton::onThemeChange(Theme::Type themeType)
{
    if (themeType == Theme::Type::LIGHT)
        this->setIcon(QPixmap(":/icon/close_black.svg"));
    else
        this->setIcon(QPixmap(":/icon/close_white.svg"));
}
F_DEFINITION_ENTEREVENT(CloseButton)
{
    if (Theme::isLigth())
        this->setIcon(QPixmap(":/icon/close_white.svg"));
    return TitleBarButton::enterEvent(event);
}

F_DEFINITION_LEAVEEVENT(CloseButton)
{
    if (Theme::isLigth())
        this->setIcon(QPixmap(":/icon/close_black.svg"));
    return TitleBarButton::leaveEvent(event);
}

F_DEFINITION_MOUSEMOVEEVENT(CloseButton)
{
    if (this->rect().contains(event->pos()))
        this->setIcon(QPixmap(":/icon/close_white.svg"));
    else
    {
        if (Theme::isLigth())
            this->setIcon(QPixmap(":/icon/close_black.svg"));
    }
    TitleBarButton::mouseMoveEvent(event);
}
