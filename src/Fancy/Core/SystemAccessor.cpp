//
// Created by TX on 2025/8/6.
//

#include "SystemAccessor.h"
#include <bcrypt.h>
#include <QDebug>
#include "Defs.h"

namespace fancy
{
    HMODULE SystemAccessor::_user32dll = GetModuleHandle(TEXT("user32.dll"));
    HMODULE SystemAccessor::_ntdll = GetModuleHandle(TEXT("ntdll.dll"));

    bool SystemAccessor::DwmEnableBlurBehindWindow(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind)
    {
        return SUCCEEDED(::DwmEnableBlurBehindWindow(hWnd, pBlurBehind));
    }

    bool SystemAccessor::SetWindowCompositionAttribute(HWND hwnd, WINDOWCOMPOSITIONATTRIBDATA *data)
    {
        if (!_user32dll)
            return false;
        const static auto SetWindowCompositionAttributePtr =
                reinterpret_cast<BOOL(WINAPI *)(HWND, WINDOWCOMPOSITIONATTRIBDATA *)>(GetProcAddress(
                    _user32dll,
                    "SetWindowCompositionAttribute"
                ));
        if (SetWindowCompositionAttributePtr)
            return static_cast<bool>(SetWindowCompositionAttributePtr(hwnd, data));
        return false;
    }

    bool SystemAccessor::DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
    {
        // 如果函数成功，则返回S_OK。否则，它将返回 HRESULT 错误代码
        // 如果已禁用桌面合成（Windows 7 及更早版本），则此函数返回DWM_E_COMPOSITIONDISABLED。
        return SUCCEEDED(::DwmSetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute));
    }

    bool SystemAccessor::DwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS *pMarInset)
    {
        return SUCCEEDED(::DwmExtendFrameIntoClientArea(hWnd, pMarInset));
    }

    bool SystemAccessor::DwmGetColorizationColor(DWORD *pcrColorization, BOOL *pfOpaqueBlend)
    {
        return SUCCEEDED(::DwmGetColorizationColor(pcrColorization, pfOpaqueBlend));
    }

    QMap<SysAccentPalette, QColor> SystemAccessor::sysAccentColors()
    {
        QMap<SysAccentPalette, QColor> colors;
        HKEY hKey = nullptr;
        // 打开注册表项
        if (RegOpenKeyEx(HKEY_CURRENT_USER,ACCENT_PALETTE_REGEDIT_PATH, 0,KEY_READ, &hKey) == ERROR_SUCCESS)
        {
            BYTE data[32] = {};
            DWORD dataSize = sizeof(data);
            DWORD valueType = 0;

            // 读取 AccentPalette 值
            if (RegQueryValueEx(hKey,ACCENT_PALETTE_KEY, nullptr, &valueType, data, &dataSize) == ERROR_SUCCESS)
            {
                if (valueType != REG_BINARY || dataSize == 32)
                    for (int i = 0; i < 7; ++i) // 解析前 7 个颜色（每 4 字节一个颜色，RGBA 顺序） int alpha = data[i * 4 + 3]
                        colors[static_cast<SysAccentPalette>(i)] = {data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2], 255};
                else
                    qWarning() << "Registry value type or size incorrect unable to obtain system color matching\nWill return empty QMap";
            }
            else
                qWarning() << "Unable to read registry values\nWill return empty QMap";
        }
        else
            qWarning() << "Unable to open registry key\nWill return empty QMap";

        RegCloseKey(hKey); //无论打开是否成功都进行关闭，关闭失败只会返回错误值，不会影响程序运行
        return colors;

        /*
         *下面的代码为另一种实现方式，依赖于 WinRT 运行时，只能使用 MSVC 编译，MinGW不支持 WinRT
                // 单次初始化检查 (线程安全)
                static std::once_flag winrtInitFlag;
                std::call_once(winrtInitFlag,
                               [] {
                                   try
                                   {
                                       // 指定STA模型
                                       winrt::init_apartment(winrt::apartment_type::single_threaded);
                                   }
                                   catch (winrt::hresult_error const &e)
                                   {
                                       qCritical() << "WinRT init failed: " << e.message().c_str();
                                   }
                               }
                );

                using namespace winrt::Windows::UI::ViewManagement;
                const UISettings uiSettings;

                // 转换函数
                auto toQColor = [](auto c) {
                    return QColor(c.R, c.G, c.B);
                };

                std::map<QString, QColor> test = {
                    {"Accent", toQColor(uiSettings.GetColorValue(UIColorType::Accent))},
                    {"AccentLight1", toQColor(uiSettings.GetColorValue(UIColorType::AccentLight1))},
                    {"AccentLight2", toQColor(uiSettings.GetColorValue(UIColorType::AccentLight2))},
                    {"AccentLight3", toQColor(uiSettings.GetColorValue(UIColorType::AccentLight3))},
                    {"AccentDark1", toQColor(uiSettings.GetColorValue(UIColorType::AccentDark1))},
                    {"AccentDark2", toQColor(uiSettings.GetColorValue(UIColorType::AccentDark2))},
                    {"AccentDark3", toQColor(uiSettings.GetColorValue(UIColorType::AccentDark3))}
                };
                for (const auto &[str,color]: test)
                    qDebug() << "[" << str << ", " << "(" << color.red() << "," << color.green() << "," << color.blue() << ")"
                            << "]";
        */
    }

    bool SystemAccessor::SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
    {
        return ::SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags) != 0;
    }

    bool SystemAccessor::SetWindowDisplayAffinity(HWND hWnd, DWORD dwAffinity)
    {
        return ::SetWindowDisplayAffinity(hWnd, dwAffinity) == TRUE;
    }

    const WindowsVersion &SystemAccessor::windowsVersion()
    {
        static const auto RtlGetVersion =
                reinterpret_cast<NTSTATUS (WINAPI*)(PRTL_OSVERSIONINFOW)>(GetProcAddress(
                    _ntdll,
                    "RtlGetVersion"
                ));

        static WindowsVersion ver;

        if (ver.MajorVersion >= 0 && ver.MinorVersion >= 0 && ver.BuildNumber >= 0)
            return ver;

        if (_ntdll)
        {
            if (RtlGetVersion)
            {
                RTL_OSVERSIONINFOW rovi = {sizeof(RTL_OSVERSIONINFOW)};
                if (!RtlGetVersion(&rovi))
                {
                    ver.MajorVersion = rovi.dwMajorVersion;
                    ver.MinorVersion = rovi.dwMinorVersion;
                    ver.BuildNumber = rovi.dwBuildNumber;
                }
                else
                    qWarning() << "Error getting Windows version information";
            }
            else
                qWarning() << "RtlGetVersion function failed to get";
        }
        else
            qWarning() << "ntdll.dll is missing";

        return ver;
    }

    bool SystemAccessor::isWindows11()
    {
        auto [Major,Minor,Build] = windowsVersion();
        return Major == 10 && Minor >= 0 && Build >= 22000;
    }

    bool SystemAccessor::isWindows10()
    {
        auto [Major,Minor,Build] = windowsVersion();
        return Major == 10 && Minor >= 0 && Build < 22000 && Build >= 0;
    }

    bool SystemAccessor::isBelowWindows10()
    {
        auto [Major,Minor,Build] = windowsVersion();
        return Major < 10 && Major >= 0 && Minor >= 0 && Build >= 0;
    }

    SystemAccessor::~SystemAccessor()
    {
        FreeLibrary(_user32dll);
        FreeLibrary(_ntdll);
    }
} // fancy
