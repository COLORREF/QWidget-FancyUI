#include "simpletitlebar.h"

SimpleTitleBar::SimpleTitleBar(QWidget *parent)
    : TitleBarBase{parent},
      minimize_button{new MinimizeButton(this)},
      maximize_button{new MaximizeButton(this)}
{
    this->horizontal_layout->insertWidget(1, this->maximize_button);
    this->horizontal_layout->insertWidget(1, this->minimize_button);
}

MinimizeButton *SimpleTitleBar::minimizeButton() const
{
    return this->minimize_button;
}

MaximizeButton *SimpleTitleBar::maximizeButton() const
{
    return this->maximize_button;
}

void SimpleTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
        this->maximize_button->click();
}
