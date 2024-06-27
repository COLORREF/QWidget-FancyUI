#include <winrt/Windows.UI.ViewManagement.h>
using namespace winrt::Windows::UI;
using namespace ViewManagement;
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <iostream>

inline bool IsColorLight(winrt::Windows::UI::Color &clr)
{
    return (((5 * clr.G) + (2 * clr.R) + clr.B) > (8 * 128));
}

COLORREF GetWindowsThemeColor()
{
    DWORD crColorization;
    BOOL fOpaqueBlend;
    COLORREF theme_color{};
    HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
    if (result == S_OK)
    {
        BYTE r, g, b;
        r = (crColorization >> 16) % 256;
        g = (crColorization >> 8) % 256;
        b = crColorization % 256;
        theme_color = RGB(r, g, b);
    }
    return theme_color;
}

void printColor(COLORREF color)
{
    std::cout << "R " << (int)GetRValue(color) << ","
              << "G " << (int)GetGValue(color) << ","
              << "B " << (int)GetBValue(color);
}

