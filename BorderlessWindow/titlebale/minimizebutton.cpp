#include "minimizebutton.h"

MinimizeButton::MinimizeButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(QPixmap(":/icon/minimize_black.svg"));
    connect(this, &MinimizeButton::clicked, this->window(), &QWidget::showMinimized);
}
