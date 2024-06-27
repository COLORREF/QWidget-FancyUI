#include "windowmanager.h"
#include <QDebug>
// #ifdef  _MSC_VER
// #ifndef Linked
// #pragma comment(lib, "user32.lib")
// #pragma comment(lib, "dwmapi.lib")
// #pragma comment(lib, "Advapi32.lib")
// #define Linked
// #endif
// #elif defined(__GNUC__)
// #endif


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
