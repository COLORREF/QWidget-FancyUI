#ifndef TRANSPARENTEFFECTWINDOWBASE_H
#define TRANSPARENTEFFECTWINDOWBASE_H

#include <QObject>
#include <QWidget>
#include "framelesswindow.h"
#include "closebutton.h"
#include "standardtitlebar.h"
#include "maximizebutton.h"
#include "minimizebutton.h"
#include "win32_utils/dwmwindoweffect.h"
#include "win32_utils/compositionwindoweffect.h"

enum class WindowEffectType
{
    ACRYLIC,//亚克力
    AREO,//毛玻璃
    FULLYTRANSPARENT,//全透明
    MICA,//云母
    MICAALT,//云母ALT
    SYSTEMCOLOR,//系统色
    DEFAULT,//默认
};


class TransparentEffectWindowBase : public FramelessWindow
{
    Q_OBJECT
protected:
    explicit TransparentEffectWindowBase(QWidget *parent, WindowEffectType effectType);
    void paintEvent(QPaintEvent *event) override;

    inline void setMicaEffect(const HWND hWnd); // 云母
    inline void setMicaAltEffect(const HWND hWnd); // 云母Alt
    inline void setAcrylicEffect(const HWND hWnd); // 亚克力
    inline void setFollowingSystemColor(const HWND hwnd); // 跟随系统颜色
    inline void setAreoEffect(const HWND hwnd); // Areo
    inline void setAreoEffect(const HWND hwnd, COLORREF abgr); // Areo,16进制颜色,每
    inline void setClientAreaFullyTransparent(const HWND hwnd); // 客户区全透明
    WindowEffectType effect_type;
private:
    inline void setTransparency();
};

inline void TransparentEffectWindowBase::setMicaEffect(const HWND hWnd)
{
    this->setTransparency();
    DwmWindowEffect::setMicaEffect(hWnd);
    MARGINS margins = {INT_MAX,0,0,0};
    ::DwmExtendFrameIntoClientArea(hWnd, &margins);
}

inline void TransparentEffectWindowBase::setMicaAltEffect(const HWND hWnd)
{
    this->setTransparency();
    DwmWindowEffect::setMicaAltEffect(hWnd);
    MARGINS margins = {INT_MAX,0,0,0};
    ::DwmExtendFrameIntoClientArea(hWnd, &margins);
}

inline void TransparentEffectWindowBase::setAcrylicEffect(const HWND hWnd)
{
    this->setTransparency();
    DwmWindowEffect::setAcrylicEffect(hWnd);
    MARGINS margins = {INT_MAX,0,0,0};
    ::DwmExtendFrameIntoClientArea(hWnd, &margins);
}

inline void TransparentEffectWindowBase::setFollowingSystemColor(const HWND hwnd)
{
    this->setTransparency();
    MARGINS margins = {0,0,1,0};
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    CompositionWindowEffect::setFollowingSystemColor((HWND)(hwnd));
}

inline void TransparentEffectWindowBase::setAreoEffect(const HWND hwnd)
{
    this->setTransparency();
    MARGINS margins = {0,0,1,0};
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    CompositionWindowEffect::setAreoEffect((HWND)(hwnd));
}

inline void TransparentEffectWindowBase::setAreoEffect(const HWND hwnd, COLORREF abgr)
{
    this->setTransparency();
    MARGINS margins = {0,0,1,0};
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    CompositionWindowEffect::setAreoEffect(hwnd, abgr);
}

inline void TransparentEffectWindowBase::setClientAreaFullyTransparent(const HWND hwnd)
{
    this->setTransparency();
    MARGINS margins = {0,0,1,0};
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    CompositionWindowEffect::setClientAreaFullyTransparent(hwnd);
}

inline void TransparentEffectWindowBase::setTransparency()
{
    ((MinimizeButton *)(((StandardTitleBar *)(this->_titleBar))->minimizeButton()))->setIsClearBeforeNewPaint(true);
    ((StandardTitleBar *)(this->_titleBar))->closeButton()->setIsClearBeforeNewPaint(true);
    ((MaximizeButton *)(((StandardTitleBar *)(this->_titleBar))->maximizeButton()))->setIsClearBeforeNewPaint(true);
    (((StandardTitleBar *)(this->_titleBar))->titleLabel())->setIsClearBeforeNewPaint(true);
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
    this->setWindowFlags((this->windowFlags()) & (~Qt::WindowType::FramelessWindowHint));
#endif
    this->setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
    if (this->_themeColorAnimation)
    {
        delete this->_themeColorAnimation;
        _themeColorAnimation = nullptr;
    }
}

#endif // TRANSPARENTEFFECTWINDOWBASE_H
