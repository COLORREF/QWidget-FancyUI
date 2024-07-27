#include "dwmwindoweffect.h"

bool DwmWindowEffect::setMicaEffect(const HWND hWnd)
{
#ifdef _MSC_VER
    MARGINS margins{-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
#elif defined(__GNUC__)
    MARGINS margins{-1,-1,-1,-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    int system_backdrop_type = 2;
    return DwmSetWindowAttribute_(
        hWnd,
        38,
        &system_backdrop_type,
        sizeof(int));
#endif
}

bool DwmWindowEffect::setMicaAltEffect(const HWND hWnd)
{
#ifdef _MSC_VER
    MARGINS margins{-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
#elif defined(__GNUC__)
    MARGINS margins{-1,-1,-1,-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    int system_backdrop_type = 4;
    return DwmSetWindowAttribute_(
        hWnd,
        38,
        &system_backdrop_type,
        sizeof(int));
#endif
}

bool DwmWindowEffect::setAcrylicEffect(const HWND hWnd)
{
#ifdef _MSC_VER
    MARGINS margins{-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
#elif defined(__GNUC__)
    MARGINS margins{-1,-1,-1,-1};
    DwmExtendFrameIntoClientArea_(hWnd, &margins);
    int system_backdrop_type = 3;
    return DwmSetWindowAttribute_(
        hWnd,
        38,
        &system_backdrop_type,
        sizeof(int));
#endif
}

bool DwmWindowEffect::setTitleBarColor(const HWND hWnd, COLORREF rgb)
{
#ifdef _MSC_VER
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
        &rgb,
        sizeof(COLORREF));
#elif defined(__GNUC__)
    return DwmSetWindowAttribute_(
        hWnd,
        35,
        &rgb,
        sizeof(COLORREF));
#endif
}

bool DwmWindowEffect::setHeaderTextColor(const HWND hWnd, COLORREF rgb)
{
#ifdef _MSC_VER
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR,
        &rgb,
        sizeof(COLORREF));
#elif defined(__GNUC__)
    return DwmSetWindowAttribute_(
        hWnd,
        36,
        &rgb,
        sizeof(COLORREF));
#endif
}

bool DwmWindowEffect::setBorderColor(const HWND hWnd, COLORREF rgb)
{
#ifdef _MSC_VER
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR,
        &rgb,
        sizeof(COLORREF));
#elif defined(__GNUC__)
    return DwmSetWindowAttribute_(
        hWnd,
        34,
        &rgb,
        sizeof(COLORREF));
#endif
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
#ifdef _MSC_VER
    BOOL value = TRUE;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL));
#elif defined(__GNUC__)
    BOOL value = TRUE;
    return DwmSetWindowAttribute_(
        hWnd,
        20,
        &value,
        sizeof(BOOL));
#endif
}

bool DwmWindowEffect::setWindowLightMode(const HWND hWnd)
{
#ifdef _MSC_VER
    BOOL value = FALSE;
    return DwmSetWindowAttribute_(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL));
#elif defined(__GNUC__)
    BOOL value = FALSE;
    return DwmSetWindowAttribute_(
        hWnd,
        20,
        &value,
        sizeof(BOOL));
#endif
}

bool DwmWindowEffect::setEntireWindowFullyTransparent(const HWND hWnd)
{
    DWM_BLURBEHIND blur_behind = {DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED, TRUE, NULL, TRUE};
    MARGINS margins{-1,-1,-1,-1};
    return DwmExtendFrameIntoClientArea_(hWnd, &margins) && DwmEnableBlurBehindWindow_(hWnd, &blur_behind);
}
