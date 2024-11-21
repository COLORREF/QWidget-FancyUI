#ifdef _WIN32
#include "FancyWin32.h"
#include <QDebug>

HMODULE WindowManager::user32dll = GetModuleHandle(TEXT("user32.dll"));

HMODULE WindowManager::ntdll = GetModuleHandle(TEXT("ntdll.dll"));

BOOL(WINAPI *WindowManager::SetWindowCompositionAttributePtr)(HWND, WINDOWCOMPOSITIONATTRIBDATA *) =
    (BOOL(WINAPI *)(HWND, WINDOWCOMPOSITIONATTRIBDATA *))GetProcAddress(WindowManager::user32dll, "SetWindowCompositionAttribute");

RtlGetVersionPtr WindowManager::RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(ntdll, "RtlGetVersion");


LRESULT WindowManager::moveWindow(const HWND hwnd)
{
    ReleaseCapture();
    return SendMessage_(hwnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
}

bool WindowManager::disableMaximize(const HWND hWnd)
{
    LONG_PTR value = SetWindowLongPtr_(hWnd, GWL_STYLE, GetWindowLongPtr_(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
    return value == 0 && GetLastError() != 0 ? false : !GetLastError();
}

bool WindowManager::disableMinimize(const HWND hWnd)
{
    return SetWindowLongPtr_(hWnd, GWL_STYLE, GetWindowLongPtr_(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX) == 0 && GetLastError() != 0 ? false : !GetLastError();
}

bool WindowManager::disablesResizingViaBorders(const HWND hWnd)
{
    return SetWindowLongPtr_(hWnd, GWL_STYLE, GetWindowLongPtr_(hWnd, GWL_STYLE) & ~WS_SIZEBOX) == 0 && GetLastError() != 0 ? false : !GetLastError();
}

bool WindowManager::disableTitleBar(const HWND hWnd)
{
    return SetWindowLongPtr_(hWnd, GWL_STYLE, GetWindowLongPtr_(hWnd, GWL_STYLE) & ~WS_SYSMENU & ~WS_CAPTION) == 0 && GetLastError() != 0 ? false : !GetLastError();
}

bool WindowManager::setWindowTop(const HWND hWnd)
{
    return SetWindowPos_(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool WindowManager::cancelWindowTop(const HWND hWnd)
{
    return SetWindowPos_(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool WindowManager::allowWindowCaptured(const HWND hWnd)
{
    return SetWindowDisplayAffinity_(hWnd, WDA_NONE);
}

bool WindowManager::prohibitWindowCaptured(const HWND hWnd)
{
    return SetWindowDisplayAffinity_(hWnd, WDA_EXCLUDEFROMCAPTURE);
}

const WindowsVersion &WindowManager::windowsVersion()
{
    static WindowsVersion ver;

    if(ver.MajorVersion >= 0 && ver.MinorVersion >= 0 && ver.BuildNumber >= 0)
        return ver;

    if (ntdll)
    {
        if (RtlGetVersion)
        {
            RTL_OSVERSIONINFOW rovi = { sizeof(RTL_OSVERSIONINFOW) };
            if (!RtlGetVersion(&rovi))
            {
                ver.MajorVersion = rovi.dwMajorVersion;
                ver.MinorVersion = rovi.dwMinorVersion;
                ver.BuildNumber = rovi.dwBuildNumber;
            }
            else
                qWarning()<< "Error getting Windows version information";
        }
        else
            qWarning()<< "RtlGetVersion function failed to get";
    }
    else
        qWarning()<<"ntdll.dll is missing";

    return ver;
}

bool WindowManager::isWindows11()
{
    auto [Major,Minor,Bulid] = WindowManager::windowsVersion();
    return Major == 10 && Minor >= 0 && Bulid >= 22000;
}

bool WindowManager::isWindows10()
{
    auto [Major,Minor,Bulid] = WindowManager::windowsVersion();
    return Major == 10 && Minor >= 0 && Bulid < 22000 && Bulid >= 0;
}

bool WindowManager::isWindowsVersionBelowWindows10()
{
    auto [Major,Minor,Bulid] = WindowManager::windowsVersion();
    return Major < 10 && Major >= 0 && Minor >=0 && Bulid >= 0;
}

WindowManager::~WindowManager()
{
    ::FreeLibrary(this->user32dll);
    ::FreeLibrary(this->ntdll);
}

BOOL WindowManager::SetWindowCompositionAttribute(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA *data)
{
    return SetWindowCompositionAttributePtr(hWnd, data);
}

bool WindowManager::DwmSetWindowAttribute_(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
{
    return SUCCEEDED(::DwmSetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute));
}

bool WindowManager::DwmExtendFrameIntoClientArea_(HWND hWnd, const MARGINS *pMarInset)
{
    return SUCCEEDED(::DwmExtendFrameIntoClientArea(hWnd, pMarInset));
}

LRESULT WindowManager::SendMessage_(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return SendMessage(hWnd, Msg, wParam, lParam);
}

bool WindowManager::DwmEnableBlurBehindWindow_(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind)
{
    // 从 Windows 8 开始，由于窗口的呈现方式发生了样式更改，因此调用此函数不会产生模糊效果，而是单纯的透明效果。
    return SUCCEEDED(::DwmEnableBlurBehindWindow(hWnd, pBlurBehind));
}

LONG_PTR WindowManager::SetWindowLongPtr_(HWND hWnd, int nIndex, LONG_PTR dwNewLong)
{
    SetLastError(0);
    return SetWindowLongPtr(hWnd, nIndex, dwNewLong);
}

LONG_PTR WindowManager::GetWindowLongPtr_(HWND hWnd, int nIndex)
{
    return GetWindowLongPtr(hWnd, nIndex);
}

bool WindowManager::SetWindowPos_(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
{
    return SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags) != 0;
}

bool WindowManager::SetWindowDisplayAffinity_(HWND hWnd, DWORD dwAffinity)
{
    return SetWindowDisplayAffinity(hWnd, dwAffinity) == TRUE;
}

WindowManager::WindowManager() {}

void CompositionWindowEffect::removeCompositionWindowEffect(const HWND hWnd)
{
    ACCENT_POLICY accent{ACCENT_STATE::ACCENT_DISABLED, 0, 0, 0};
    WINDOWCOMPOSITIONATTRIBDATA data{WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent, sizeof(ACCENT_POLICY)};
    SetWindowCompositionAttribute(hWnd, &data);
}

void CompositionWindowEffect::setFollowingSystemColor(const HWND hwnd)
{
    ACCENT_POLICY accent = {ACCENT_STATE::ACCENT_ENABLE_TRANSPARENTGRADIENT, 0, 0, 0};
    WINDOWCOMPOSITIONATTRIBDATA data{WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent, sizeof(ACCENT_POLICY)};
    SetWindowCompositionAttribute(hwnd, &data);
}

void CompositionWindowEffect::setAreoEffect(const HWND hwnd)
{
    ACCENT_POLICY accent = {ACCENT_STATE::ACCENT_ENABLE_AERO_BLUR, 0, 0, 0};
    WINDOWCOMPOSITIONATTRIBDATA data{WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent, sizeof(ACCENT_POLICY)};
    SetWindowCompositionAttribute(hwnd, &data);
}

void CompositionWindowEffect::setAreoEffect(const HWND hwnd, COLORREF abgr)
{
    ACCENT_POLICY accent = {ACCENT_STATE::ACCENT_ENABLE_AERO_BLUR_COLOR, 0, abgr, 0};
    WINDOWCOMPOSITIONATTRIBDATA data{WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent, sizeof(ACCENT_POLICY)};
    SetWindowCompositionAttribute(hwnd, &data);
}

void CompositionWindowEffect::setClientAreaFullyTransparent(const HWND hwnd)
{
    ACCENT_POLICY accent = {ACCENT_STATE::ACCENT_INVALID_STATE, 0, 0, 0};
    WINDOWCOMPOSITIONATTRIBDATA data{WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent, sizeof(ACCENT_POLICY)};
    SetWindowCompositionAttribute(hwnd, &data);
}


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
#endif
