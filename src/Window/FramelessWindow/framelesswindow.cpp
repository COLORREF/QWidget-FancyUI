#include "framelesswindow.h"
#include "standardtitlebar.h"

FramelessWindow::FramelessWindow(QWidget *parent)
    : SimpleFramelessWindow(parent, Type::Standard)
{
}

void FramelessWindow::setWindowIcon(const QPixmap &icon)
{
    ((StandardTitleBar *)(this->_titleBar))->setIcon(icon);
}

void FramelessWindow::setWindowTitle(const QString &title)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitle(title);
}

void FramelessWindow::setTitleFont(const QFont &font)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitleFont(font);
}

void FramelessWindow::setTitleColor(const QColor &globalColor)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitleColor(globalColor);
}

void FramelessWindow::setTitleColor(const QColor &lightColor, const QColor &darkColor)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitleColor(lightColor, darkColor);
}
