#include "ntqqwindow.h"
#include "win32_utils/dwmwindoweffect.h"
#include "win32_utils/compositionwindoweffect.h"
#include "win32_utils/windowmanager.h"
#include <QDebug>

NTQQWindow::NTQQWindow(QWidget *parent):
    SimpleFramelessWindow(parent)
{
    QWidget* sidebar_area = new QWidget(this);
    QHBoxLayout* sidbar_area_layout = new QHBoxLayout(sidebar_area);
    sidbar_area_layout->setSpacing(0);
    sidbar_area_layout->setContentsMargins(0,0,0,0);
    this->_sidebar = new LightAndDarkWidget(this,QColor(242,242,242,105),QColor(17, 17, 17, 105));
    sidbar_area_layout->addWidget(this->_sidebar);
    this->_horizontalLayout->insertWidget(0,sidebar_area);
    this->_sidebar->setMinimumWidth(58);
    this->_themeColorAnimation->setThemeColor(QColor(242,242,242),QColor(17,17,17));
    this->setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);

    if(WindowManager::isWindows11())
    {
        DwmWindowEffect::setAcrylicEffect(((HWND)this->winId()));
        MARGINS margins = {-1,-1,-1,-1};
        ::DwmExtendFrameIntoClientArea(((HWND)this->winId()), &margins);
    }
    else if(WindowManager::isWindows10())
    {
        MARGINS margins = {0, 0, 1, 0};
        ::DwmExtendFrameIntoClientArea(((HWND)this->winId()), &margins);
        CompositionWindowEffect::setClientAreaFullyTransparent(((HWND)this->winId()));
    }

    else
    {
        qWarning()
            <<"The QQNT window with fuzzy sidebar effect is only effective in Windows11 operating system,"
            <<"and the sidebar of Windows10 operating system will not have fuzzy effect."
            <<"The effect of the operating system below Windows10 is unknown, and unknown errors may occur.";
    }
}

void NTQQWindow::setUnmaskSidebarColor(bool isUnmask)
{
    if(isUnmask)
    {
        this->_sidebar->themeColorManagement()->setThemeColor(Qt::GlobalColor::transparent,Qt::GlobalColor::transparent);
        this->_sidebar->themeColorManagement()->setEnable(false);

    }
    else
    {
        this->_sidebar->themeColorManagement()->setThemeColor(QColor(242,242,242,105),QColor(17, 17, 17, 105));
        this->_sidebar->themeColorManagement()->setEnable(true);
    }
}


void NTQQWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setBrush(this->_themeColorAnimation->runTimeColor());
    painter.drawRect(QRect(this->_mainArea->mapToParent(QPoint(0,0)),this->_mainArea->size()));
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.eraseRect(QRect(this->_sidebar->mapFromParent(QPoint(0,0)) ,this->_sidebar->size()));
}
