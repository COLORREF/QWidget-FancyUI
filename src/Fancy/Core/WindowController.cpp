//
// Created by TX on 2025/8/6.
//

#include "WindowController.h"
#include "Defs.h"

namespace fancy
{
    namespace
    {
        bool removeWindowStyles(HWND hwnd, LONG_PTR mask)
        {
            SetLastError(ERROR_SUCCESS);
            const LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
            if (style == 0 && GetLastError() != ERROR_SUCCESS)
                return false;

            SetLastError(ERROR_SUCCESS);
            const LONG_PTR previousStyle = SetWindowLongPtr(hwnd, GWL_STYLE, style & ~mask);
            return previousStyle != 0 || GetLastError() == ERROR_SUCCESS;
        }
    }

    LRESULT WindowController::moveWindow(HWND hWnd)
    {
        ReleaseCapture();
        return SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    }

    bool WindowController::disableMaximize(HWND hWnd)
    {
        return removeWindowStyles(hWnd, WS_MAXIMIZEBOX);
    }

    bool WindowController::enableFramelessWindowStyles(HWND hWnd)
    {
        SetLastError(ERROR_SUCCESS);
        const LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
        if (style == 0 && GetLastError() != ERROR_SUCCESS)
            return false;
        SetLastError(ERROR_SUCCESS);
        const LONG_PTR previousStyle = SetWindowLongPtr(hWnd, GWL_STYLE, style | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU);
        return previousStyle != 0 || GetLastError() == ERROR_SUCCESS; // 返回零不一定失败，不能只根据旧样式值判断
    }

    LRESULT WindowController::updateNonClientActivation(HWND hWnd, bool active)
    {
        // lParam = -1 保留系统激活状态处理，但禁止重绘自绘窗口的原生标题栏
        return DefWindowProc(hWnd, WM_NCACTIVATE, active ? TRUE : FALSE, -1);
    }

    bool WindowController::disableMinimize(HWND hWnd)
    {
        return removeWindowStyles(hWnd, WS_MINIMIZEBOX);
    }

    bool WindowController::disablesResizingViaBorders(HWND hWnd)
    {
        return removeWindowStyles(hWnd, WS_SIZEBOX);
    }

    bool WindowController::disableTitleBar(HWND hWnd)
    {
        return removeWindowStyles(hWnd, WS_SYSMENU | WS_CAPTION);
    }

    bool WindowController::setWindowTop(HWND hWnd, bool top)
    {
        if (top)
            return SystemAccessor::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        return SystemAccessor::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    bool WindowController::setCompositionWindowEffect(HWND hWnd, CompositionWindowEffect effect, COLORREF abgr)
    {
        ACCENT_POLICY accent{};
        DWORD flags = 0;
        COLORREF color = 0;

        switch (effect)
        {
            case CompositionWindowEffect::Disabled :
                accent.AccentState = ACCENT_STATE::ACCENT_DISABLED;
                break;
            case CompositionWindowEffect::SolidColor :
                accent.AccentState = ACCENT_STATE::ACCENT_ENABLE_GRADIENT;
                color = abgr;
                break;
            case CompositionWindowEffect::SystemColor :
                accent.AccentState = ACCENT_STATE::ACCENT_ENABLE_TRANSPARENTGRADIENT;
                break;
            case CompositionWindowEffect::TransparentColor :
                accent.AccentState = ACCENT_STATE::ACCENT_ENABLE_TRANSPARENTGRADIENT;
                flags = 2;
                color = abgr;
                break;
            case CompositionWindowEffect::Areo :
                accent.AccentState = ACCENT_STATE::ACCENT_ENABLE_BLURBEHIND;
                break;
            case CompositionWindowEffect::AreoWithColor :
                accent.AccentState = ACCENT_STATE::ACCENT_ENABLE_ACRYLICBLURBEHIND;
                color = abgr;
                break;
            case CompositionWindowEffect::FullyTransparent :
                accent.AccentState = ACCENT_STATE::ACCENT_INVALID_STATE;
                break;
            default :
                return false;
        }

        accent.AccentFlags = flags;
        accent.GradientColor = color;
        accent.AnimationId = 0;

        WINDOWCOMPOSITIONATTRIBDATA data{
            WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY,
            &accent,
            sizeof(ACCENT_POLICY)
        };
        return SystemAccessor::SetWindowCompositionAttribute(hWnd, &data);
    }

    bool WindowController::setDwmWindowEffect(HWND hWnd, DwmWindowEffect effect)
    {
        win32::dwm_systembackdrop_type system_backdrop_type;

        switch (effect)
        {
            case DwmWindowEffect::Acrylic :
                system_backdrop_type = win32::dwm_systembackdrop_type::dwmsbt_transientwindow;
                break;
            case DwmWindowEffect::Mica :
                system_backdrop_type = win32::dwm_systembackdrop_type::dwmsbt_mainwindow;
                break;
            case DwmWindowEffect::MicaAlt :
                system_backdrop_type = win32::dwm_systembackdrop_type::dwmsbt_tabbedwindow;
                break;
            case DwmWindowEffect::Auto :
                system_backdrop_type = win32::dwm_systembackdrop_type::dwmsbt_auto;
                break;
            case DwmWindowEffect::None :
                system_backdrop_type = win32::dwm_systembackdrop_type::dwmsbt_none;
                break;
            default :
                return false;
        }

        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            win32::dwmwa_systembackdrop_type,
            &system_backdrop_type,
            sizeof(system_backdrop_type)
        );
    }

    bool WindowController::extendFrameIntoClientArea(HWND hWnd, MARGINS margins)
    {
        return SystemAccessor::DwmExtendFrameIntoClientArea(hWnd, &margins);
    }

    bool WindowController::setNonClientDwmRenderingStrategy(HWND hWnd, bool enable)
    {
        DWMNCRENDERINGPOLICY value;
        if (enable)
            value = DWMNCRP_ENABLED;
        else
            value = DWMNCRP_DISABLED;
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            DWMWA_NCRENDERING_POLICY,
            &value,
            sizeof(value)
        );
    }

    bool WindowController::setBorderColor(HWND hWnd, COLORREF rgb)
    {
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            win32::dwmwa_border_color,
            &rgb,
            sizeof(rgb)
        );
    }

    bool WindowController::setTitleTextColor(HWND hWnd, COLORREF rgb)
    {
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            win32::dwmwa_text_color,
            &rgb,
            sizeof(rgb)
        );
    }

    bool WindowController::setTitleBarColor(HWND hWnd, COLORREF rgb)
    {
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            win32::dwmwa_caption_color,
            &rgb,
            sizeof(rgb)
        );
    }

    bool WindowController::setWindowThemeMode(HWND hWnd, Theme theme)
    {
        const BOOL dark = theme == Theme::Dark;
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            win32::dwmwa_use_immersive_dark_mode,
            &dark,
            sizeof(dark)
        );
    }

    bool WindowController::setWindowCaptureStrategy(HWND hWnd, bool capture, bool hide)
    {
        if (capture)
            return SystemAccessor::SetWindowDisplayAffinity(hWnd, WDA_NONE);
        if (hide)
            return SystemAccessor::SetWindowDisplayAffinity(hWnd,WDA_EXCLUDEFROMCAPTURE);
        return SystemAccessor::SetWindowDisplayAffinity(hWnd, WDA_MONITOR);
    }

    bool WindowController::enableBlurBehindWindow(HWND hWnd)
    {
        DWM_BLURBEHIND blur_behind = {
            DWM_BB_ENABLE | DWM_BB_BLURREGION | DWM_BB_TRANSITIONONMAXIMIZED,
            TRUE,
            nullptr,
            TRUE
        };
        return SystemAccessor::DwmEnableBlurBehindWindow(hWnd, &blur_behind);
    }
} // fancy
