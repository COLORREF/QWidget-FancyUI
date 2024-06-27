#include "toolbutton.h"


ToolButton::ToolButton(QWidget *parent, ToolType type):
    TitleBarButton{parent}
{
    switch (type)
    {
    case ToolType::CUSTOM:
        break;
    case ToolType::GEAR:
        this->light = QPixmap(":/icon/gear_black.svg");
        this->dark = QPixmap(":/icon/gear_white.svg");
        break;
    case ToolType::WRENCH:
       this->light = QPixmap(":/icon/wrench_black.svg");
        this->dark =QPixmap(":/icon/wrench_white.svg");
        break;
    case ToolType::QUESTION:
        this->light = QPixmap(":/icon/question_black.svg");
        this->dark =QPixmap(":/icon/question_white.svg");
        break;
    case ToolType::SEARCH:
        this->light = QPixmap(":/icon/search_black.svg");
        this->dark = QPixmap(":/icon/search_white.svg");
        break;
    case ToolType::WIFI:
        this->light = QPixmap(":/icon/wifi_black.svg");
        this->dark = QPixmap(":/icon/wifi_white.svg");
        break;
    case ToolType::BLUETOOTH:
        this->light = QPixmap(":/icon/bluetooth_black.svg");
        this->dark =QPixmap(":/icon/bluetooth_white.svg");
        break;
    case ToolType::THREE_HORIZONTAL_LINES:
        this->light = QPixmap(":/icon/three_horizontal_lines_black.svg");
        this->dark = QPixmap(":/icon/three_horizontal_lines_white.svg");
        break;
    case ToolType::THREE_HORIZONTAL_POINTS:
        this->light = QPixmap(":/icon/three_horizontal_points_black.svg");
        this->dark =QPixmap(":/icon/three_horizontal_points_white.svg");
        break;
    case ToolType::SUN_MOON:
        this->light = QPixmap(":/icon/sun_moon_black.svg");
        this->dark =QPixmap(":/icon/sun_moon_white.svg");
        break;
    case ToolType::LEFT_ARROW:
        this->light = QPixmap(":/icon/left_arrow_black.svg");
        this->dark =QPixmap(":/icon/left_arrow_white.svg");
    }
    if(type != ToolType::CUSTOM)
        this->setIcon(this->light);
    this->type = type;
}

void ToolButton::setThemeIcon(const QPixmap& light, const QPixmap& dark)
{
    this->light = light;
    this->dark = dark;
}

void ToolButton::setFixedSize(const QSize &size)
{
    TitleBarButton::setFixedSize(size);
    this->_iconLabel->move((this->width()-this->_iconLabel->width())/2, (this->height()-this->_iconLabel->height())/2);
}

void ToolButton::setFixedSize(int w, int h)
{
    this->setFixedSize(QSize(w,h));
}

void ToolButton::setFixedHeight(int h)
{
    this->setFixedSize(this->width(),h);

}

void ToolButton::setFixedWidth(int w)
{
    this->setFixedSize(w,this->height());
}

void ToolButton::resizeIconSize(const QSize &size)
{
    this->_iconLabel->resize(size);
}

void ToolButton::cancelsFixedSize()
{
    this->setMinimumSize(QSize(0, 0)); // 重置最小尺寸
    this->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)); // 重置最大尺寸
}

void ToolButton::onThemeChange(Theme::Type themeType)
{
    if (themeType == Theme::Type::LIGHT)
    {
        if(this->type != ToolType::CUSTOM)
            this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05));
        if(!this->light.isNull())
            this->setIcon(QPixmap(this->light));
    }
    else
    {
        if(this->type != ToolType::CUSTOM)
            this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.2));
        if(!this->dark.isNull())
            this->setIcon(QPixmap(this->dark));
    }
}
