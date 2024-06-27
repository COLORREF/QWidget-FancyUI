#include "simpletitlebar.h"

SimpleTitleBar::SimpleTitleBar(QWidget *parent)
    : TitleBarBase{parent},
      _minimizeButton{new MinimizeButton(this)},
      _maximizeButton{new MaximizeButton(this)}
{
    this->_horizontalLayout->insertWidget(1, this->_maximizeButton);
    this->_horizontalLayout->insertWidget(1, this->_minimizeButton);
}

void SimpleTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        this->_maximizeButton->click();
        if (window()->isMaximized())
            window()->showNormal();
        else
            window()->showMaximized();
    }
}
