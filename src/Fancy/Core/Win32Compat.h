#ifndef QWIDGET_FANCYUI_WIN32COMPAT_H
#define QWIDGET_FANCYUI_WIN32COMPAT_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <dwmapi.h>

namespace fancy::win32
{
    // https://learn.microsoft.com/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
    inline constexpr DWORD dwmwa_use_immersive_dark_mode = 20;
    inline constexpr DWORD dwmwa_border_color = 34;
    inline constexpr DWORD dwmwa_caption_color = 35;
    inline constexpr DWORD dwmwa_text_color = 36;
    inline constexpr DWORD dwmwa_systembackdrop_type = 38;

    inline constexpr COLORREF dwmwa_color_default = 0xFFFFFFFFu;
    inline constexpr COLORREF dwmwa_color_none = 0xFFFFFFFEu;

    // https://learn.microsoft.com/windows/win32/api/dwmapi/ne-dwmapi-dwm_systembackdrop_type
    enum class dwm_systembackdrop_type : int
    {
        dwmsbt_auto = 0,
        dwmsbt_none = 1,
        dwmsbt_mainwindow = 2,
        dwmsbt_transientwindow = 3,
        dwmsbt_tabbedwindow = 4
    };

    static_assert(sizeof(dwm_systembackdrop_type) == 4,
                  "dwm_systembackdrop_type 必须为 4 字节，以匹配 DwmSetWindowAttribute 要求的 Windows ABI\n"
                  "dwm_systembackdrop_type must be 4 bytes to match the Windows ABI required by DwmSetWindowAttribute.");
    // 这些定义用于兼容旧版头文件；运行时仍需检查系统是否支持相应功能，并检查 DwmSetWindowAttribute 的调用结果
}
#endif

#endif // QWIDGET_FANCYUI_WIN32COMPAT_H
