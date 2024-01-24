#include "windoweffect.h"
#pragma comment(lib,"user32.lib")
#pragma comment(lib, "dwmapi.lib")

BOOL(WINAPI* WindowEffect::SetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*)
    {(BOOL(WINAPI*)(HWND, WINDOWCOMPOSITIONATTRIBDATA*))GetProcAddress(GetModuleHandleW(L"user32.dll"), "SetWindowCompositionAttribute")};

WindowEffect::WindowEffect()
{}

void WindowEffect::setFollowingSystemColor(const HWND hwnd)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_ENABLE_TRANSPARENTGRADIENT, 0, 0, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}

void WindowEffect::setAreoEffect(const HWND hwnd)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_ENABLE_AERO_BLUR, 0, 0, 1 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}

void WindowEffect::setAreoEffect(const HWND hwnd, COLORREF abgr)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_ENABLE_AERO_BLUR_COLOR, 0, abgr, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}

void WindowEffect::setFullTransparency(const HWND hwnd)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_INVALID_STATE, 0, 0, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}

void WindowEffect::setMicaEffect(const HWND hwnd)
{
    MARGINS margins{ -1, -1, -1, -1 };
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
    ::DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE, &system_backdrop_type, sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

void WindowEffect::setMicaAltEffect(const HWND hwnd)
{
    MARGINS margins{ -1, -1, -1, -1 };
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW;
    ::DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE, &system_backdrop_type, sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

void WindowEffect::setAcrylicEffect(const HWND hwnd)
{
    MARGINS margins{ -1, -1, -1, -1 };
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW;
    ::DwmSetWindowAttribute(hwnd, DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE, &system_backdrop_type, sizeof(DWM_SYSTEMBACKDROP_TYPE));
}


void WindowEffect::moveWindow(const HWND hwnd)
{
    ReleaseCapture();
    SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
}

bool WindowEffect::setTitleBarColor(const HWND hwnd, int red, int green, int blue)
{
    COLORREF color = RGB(red, green, blue);
    return SUCCEEDED(::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
        &color,
        sizeof(COLORREF)));
}

bool WindowEffect::setWindowDarkMode(const HWND hwnd)
{
    BOOL value = TRUE;
    return SUCCEEDED(::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL)));// 返回设置或取消深色模式是否成功
}

bool WindowEffect::setWindowLightMode(const HWND hwnd)
{
    BOOL value = FALSE;
    return SUCCEEDED(::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL)));// 返回设置或取消深色模式是否成功
}
