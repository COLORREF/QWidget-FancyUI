#include "windoweffect.h"

//下面两条预处理是MSVC的语法，若使用MinGW编译，请修改配置文件，在配置文件中链接库，并删去这两句预处理命令
//参考：https://blog.csdn.net/qqwangfan/article/details/105512532
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
    ::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

void WindowEffect::setMicaAltEffect(const HWND hwnd)
{
    MARGINS margins{ -1, -1, -1, -1 };
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW;
    ::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

void WindowEffect::setAcrylicEffect(const HWND hwnd)
{
    MARGINS margins{ -1, -1, -1, -1 };
    ::DwmExtendFrameIntoClientArea(hwnd, &margins);
    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TRANSIENTWINDOW;
    ::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE,
        &system_backdrop_type,
        sizeof(DWM_SYSTEMBACKDROP_TYPE));
}


void WindowEffect::moveWindow(const HWND hwnd)
{
    ReleaseCapture();
    SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
}

bool WindowEffect::setTitleBarColor(const HWND hwnd, COLORREF rgb)
{
    return SUCCEEDED(::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
        &rgb,
        sizeof(COLORREF)));
}

bool WindowEffect::setWindowDarkMode(const HWND hwnd)
{
    BOOL value = TRUE;
    return SUCCEEDED(::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL)));
}

bool WindowEffect::setWindowLightMode(const HWND hwnd)
{
    BOOL value = FALSE;
    return SUCCEEDED(::DwmSetWindowAttribute(
        hwnd,
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL)));
}

bool WindowEffect::setHeaderTextColor(const HWND hWnd, COLORREF rgb)
{
    return SUCCEEDED(::DwmSetWindowAttribute(
        hWnd,
        DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR,
        &rgb,
        sizeof(COLORREF)));
}

bool WindowEffect::setBorderColor(const HWND hWnd, COLORREF rgb)
{
    return SUCCEEDED(::DwmSetWindowAttribute(
        hWnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR,
        &rgb,
        sizeof(COLORREF)));
}
