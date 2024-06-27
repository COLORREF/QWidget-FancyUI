#include "areowindow.h"
#include "win32_utils/compositionwindoweffect.h"

AreoWindow::AreoWindow(QWidget *parent)
    : TransparentEffectWindowBase{parent,WindowEffectType::AREO}
{
    this->init();
}

AreoWindow::AreoWindow(COLORREF abgr, QWidget *parent)
    : TransparentEffectWindowBase{parent,WindowEffectType::AREO}
{
    this->init(abgr);
}

void AreoWindow::changeColor(COLORREF abgr)
{
    if(WindowManager::isWindows10() || WindowManager::isWindows11())
    {
        if (this->window_color < 0)
        {
            if (abgr)
            {
                CompositionWindowEffect::removeCompositionWindowEffect((HWND)(this->winId()));
                this->setAreoEffect((HWND)(this->winId()), abgr);
                this->window_color = abgr;
            }
            else
                return;
        }
        else
        {
            if (abgr == this->window_color)
                return;
            CompositionWindowEffect::removeCompositionWindowEffect((HWND)(this->winId()));
            if (abgr)
            {
                this->setAreoEffect((HWND)(this->winId()), abgr);
                this->window_color = abgr;
            }
            else
            {
                this->setAreoEffect((HWND)(this->winId()));
                this->window_color = -1;
            }
        }
    }
}

void AreoWindow::init(long long abgr)
{
    if(WindowManager::isWindows10() || WindowManager::isWindows11())
    {
        if (abgr < 0)
            this->setAreoEffect((HWND)(this->winId()));
        else
        {
            this->setAreoEffect((HWND)(this->winId()), abgr);
            this->window_color = abgr;
        }
    }
}
