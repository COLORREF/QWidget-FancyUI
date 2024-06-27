#include "transparenteffectwindowbase.h"
#include "win32_utils/windowmanager.h"
#include <QDebug>

TransparentEffectWindowBase::TransparentEffectWindowBase(QWidget *parent, WindowEffectType effectType):
    FramelessWindow{parent},
    effect_type{effectType}
{
    bool isWindows11 = WindowManager::isWindows11();
    bool isWindowsVersionBelowWindows10 = WindowManager::isWindowsVersionBelowWindows10();

    if(isWindowsVersionBelowWindows10)
    {
        qWarning()<<"Operating systems younger than Windows10 May not be supported or may have unknown errors";
        qWarning()<<"The window has been initialized as the default window";
        this->effect_type = WindowEffectType::DEFAULT;
    }

    switch (effect_type)
    {
    case WindowEffectType::ACRYLIC://亚克力，仅支持 Windows11
        if(isWindows11)
            this->setAcrylicEffect((HWND)this->winId());
        else
        {
            qWarning()<<"The acrylic effect is only supported in Windows11";
            qWarning()<<"The window has been initialized as the default window";
            this->effect_type = WindowEffectType::DEFAULT;
        }
        return;
    case WindowEffectType::AREO:return;//毛玻璃的初始化将在毛玻璃窗口类中（子类）中完成，支持 Windows10 和 Windows11
    case WindowEffectType::FULLYTRANSPARENT://全透明，支持 Windows10 和 Windows11
        this->setClientAreaFullyTransparent((HWND)this->winId());
        return;
    case WindowEffectType::MICA://云母，仅支持 Windows11
        if(isWindows11)
            this->setMicaEffect((HWND)this->winId());
        else
        {
            qWarning()<<"The mica effect is only supported in Windows11";
            qWarning()<<"The window has been initialized as the default window";
            this->effect_type = WindowEffectType::DEFAULT;
        }
        return;
    case WindowEffectType::MICAALT://云母alt，仅支持 Windows11
        if(isWindows11)
            this->setMicaAltEffect((HWND)this->winId());
        else
        {
            qWarning()<<"The mica alt effect is only supported in Windows11";
            qWarning()<<"The window has been initialized as the default window";
            this->effect_type = WindowEffectType::DEFAULT;
        }
        return;
    case WindowEffectType::SYSTEMCOLOR://系统色，支持 Windows10 和 Windows11
        this->setFollowingSystemColor((HWND)this->winId());
        return;
    case WindowEffectType::DEFAULT://默认窗口
        return;
    }
}

void TransparentEffectWindowBase::paintEvent(QPaintEvent *event)
{
    if(this->effect_type != WindowEffectType::DEFAULT)
    {
        QPainter painter(this);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
        return QWidget::paintEvent(event);
    }
    FramelessWindow::paintEvent(event);
}


