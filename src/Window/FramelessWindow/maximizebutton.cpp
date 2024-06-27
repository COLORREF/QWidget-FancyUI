#include "maximizebutton.h"

MaximizeButton::MaximizeButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(QPixmap(":/icon/maximize_black.svg"));
    connect(this, &MaximizeButton::enter, this, &MaximizeButton::onEnter);
    connect(this, &MaximizeButton::leave, this, &MaximizeButton::onLeave);
    connect(this, &MaximizeButton::mousePress, this, &MaximizeButton::onMousePress);
    connect(this, &MaximizeButton::mouseRelease, this, &MaximizeButton::onMouseRelease);
    connect(this, &MaximizeButton::mouseMove, this, &MaximizeButton::onMouseMove);
    connect(this, &MaximizeButton::stateChange, this, &MaximizeButton::onStateChange);
}

MaximizeButton::State MaximizeButton::maxmizeState()
{
    return this->state;
}

void MaximizeButton::onThemeChange(Theme::Type themeType)
{
    if (themeType == Theme::Type::LIGHT)
    {
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05)); // 浅色背景点击时颜色减淡，声色背景颜色加深
        if (this->state == State::NORMAL)
            this->setIcon(QPixmap(":/icon/maximize_black.svg"));
        else
            this->setIcon(QPixmap(":/icon/restore_black.svg"));
    }
    else
    {
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.2));
        if (this->state == State::NORMAL)
            this->setIcon(QPixmap(":/icon/maximize_white.svg"));
        else
            this->setIcon(QPixmap(":/icon/restore_white.svg"));
    }
}

void MaximizeButton::onStateChange(MaximizeButton::State state)
{
    if (state == State::NORMAL)
    {
        if (Theme::isLigth())
            this->setIcon(QPixmap(":/icon/maximize_black.svg"));
        else
            this->setIcon(QPixmap(":/icon/maximize_white.svg"));
    }
    else
    {
        if (Theme::isLigth())
            this->setIcon(QPixmap(":/icon/restore_black.svg"));
        else
            this->setIcon(QPixmap(":/icon/restore_white.svg"));
    }
}

void MaximizeButton::onEnter()
{
    this->setMouseState(TitleBarButton::MouseState::HOVER);
}

void MaximizeButton::onLeave()
{
    this->setMouseState(TitleBarButton::MouseState::NORMAL);
}

void MaximizeButton::onMousePress()
{
    this->setMouseState(TitleBarButton::MouseState::PRESSEDHOVER);
}

void MaximizeButton::onMouseRelease(bool isInSelf)
{
    if (isInSelf)
        this->setMouseState(TitleBarButton::MouseState::HOVER);
    else
        this->setMouseState(TitleBarButton::MouseState::NORMAL);
}

void MaximizeButton::onMouseMove(bool isInSelf)
{
    if (isInSelf)
        this->setMouseState(TitleBarButton::MouseState::PRESSEDHOVER);
    else
        this->setMouseState(TitleBarButton::MouseState::PRESSEDLEAVE);
}
