#include "minimizebutton.h"

MinimizeButton::MinimizeButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(QPixmap(":/icon/minimize_black.svg"));
    connect(this, &MinimizeButton::clicked, this->window(), &QWidget::showMinimized);
}

void MinimizeButton::onThemeChange(Theme::Type themeType)
{
    if (themeType == Theme::Type::LIGHT)
    {
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05));
        this->setIcon(QPixmap(":/icon/minimize_black.svg"));
    }
    else
    {
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.2));
        this->setIcon(QPixmap(":/icon/minimize_white.svg"));
    }
}
