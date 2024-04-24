#include "dwmwindoweffect.h"

bool DwmWindowEffect::setMicaEffect(const HWND hWnd)
{
    MARGINS margins{-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

bool DwmWindowEffect::setMicaAltEffect(const HWND hWnd)
{
    MARGINS margins{ -1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

bool DwmWindowEffect::setAcrylicEffect(const HWND hWnd)
{
    MARGINS margins{-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

bool DwmWindowEffect::setTitleBarColor(const HWND hWnd, COLORREF rgb)
{
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
        &rgb,
        sizeof(COLORREF));
}

bool DwmWindowEffect::setHeaderTextColor(const HWND hWnd, COLORREF rgb)
{
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR,
        &rgb,
        sizeof(COLORREF));
}

bool DwmWindowEffect::setBorderColor(const HWND hWnd, COLORREF rgb)
{
    return DwmSetWindowAttribute_(
        hWnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR,
        &rgb,
        sizeof(COLORREF));
}

bool DwmWindowEffect::disabledNonClientRendering(const HWND hWnd)
{
    DWMNCRENDERINGPOLICY value = DWMNCRENDERINGPOLICY::DWMNCRP_DISABLED;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_NCRENDERING_POLICY,
        &value,
        sizeof(DWMWINDOWATTRIBUTE));
}

bool DwmWindowEffect::enabledNonClientRendering(const HWND hWnd)
{
    DWMNCRENDERINGPOLICY value = DWMNCRENDERINGPOLICY::DWMNCRP_ENABLED;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_NCRENDERING_POLICY,
        &value,
        sizeof(DWMWINDOWATTRIBUTE));
}

bool DwmWindowEffect::setWindowDarkMode(const HWND hWnd)
{
    BOOL value = TRUE;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL));
}

bool DwmWindowEffect::setWindowLightMode(const HWND hWnd)
{
    BOOL value = FALSE;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL));
}

bool DwmWindowEffect::setEntireWindowFullyTransparent(const HWND hWnd)
{
    DWM_BLURBEHIND blur_behind = {DWM_BB_ENABLE|DWM_BB_BLURREGION|DWM_BB_TRANSITIONONMAXIMIZED,TRUE,NULL,TRUE};
    MARGINS margins = {-1};
    return DwmExtendFrameIntoClientArea_(hWnd,&margins) && DwmEnableBlurBehindWindow_(hWnd,&blur_behind);
}
