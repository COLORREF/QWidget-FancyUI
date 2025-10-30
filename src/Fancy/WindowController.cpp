//
// Created by TX on 2025/8/6.
//

#include "WindowController.h"
#include "Defs.hpp"

namespace fancy
{
    LRESULT WindowController::moveWindow(HWND hWnd)
    {
        ReleaseCapture();
        return SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    }

    bool WindowController::disableMaximize(HWND hWnd)
    {
        LONG_PTR value = SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
        return value == 0 && GetLastError() != 0 ? false : !GetLastError();
    }

    bool WindowController::disableMinimize(HWND hWnd)
    {
        LONG_PTR value = SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
        return value == 0 && GetLastError() != 0 ? false : !GetLastError();
    }

    bool WindowController::disablesResizingViaBorders(HWND hWnd)
    {
        LONG_PTR value = SetWindowLongPtr(hWnd, GWL_STYLE, GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
        return value == 0 && GetLastError() != 0 ? false : !GetLastError();
    }

    bool WindowController::disableTitleBar(HWND hWnd)
    {
        LONG_PTR value = SetWindowLongPtr(hWnd,GWL_STYLE,GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_SYSMENU & ~WS_CAPTION);
        return value == 0 && GetLastError() != 0 ? false : !GetLastError();
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
        DWM_SYSTEMBACKDROP_TYPE system_backdrop_type;

        switch (effect)
        {
            case DwmWindowEffect::Acrylic :
                system_backdrop_type = DWMSBT_TRANSIENTWINDOW;
                break;
            case DwmWindowEffect::Mica :
                system_backdrop_type = DWMSBT_MAINWINDOW;
                break;
            case DwmWindowEffect::MicaAlt :
                system_backdrop_type = DWMSBT_TABBEDWINDOW;
                break;
            case DwmWindowEffect::Auto :
                system_backdrop_type = DWMSBT_AUTO;
                break;
            case DwmWindowEffect::None :
                system_backdrop_type = DWMSBT_NONE;
                break;
            default :
                return false;
        }

        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            DWMWA_SYSTEMBACKDROP_TYPE,
            &system_backdrop_type,
            sizeof(DWM_SYSTEMBACKDROP_TYPE)
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
            sizeof(DWMWINDOWATTRIBUTE)
        );
    }

    bool WindowController::setBorderColor(HWND hWnd, COLORREF rgb)
    {
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            DWMWA_BORDER_COLOR,
            &rgb,
            sizeof(COLORREF)
        );
    }

    bool WindowController::setTitleTextColor(HWND hWnd, COLORREF rgb)
    {
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            DWMWA_TEXT_COLOR,
            &rgb,
            sizeof(COLORREF)
        );
    }

    bool WindowController::setTitleBarColor(HWND hWnd, COLORREF rgb)
    {
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            DWMWA_CAPTION_COLOR,
            &rgb,
            sizeof(COLORREF)
        );
    }

    bool WindowController::setWindowThemeMode(HWND hWnd, Theme theme)
    {
        // theme 的值恰好与此处 DwmSetWindowAttribute 所需的 BOOL 变量值对应，故为简化处理逻辑此处直接对 theme 取指，不再创建 BOOL 变量
        return SystemAccessor::DwmSetWindowAttribute(
            hWnd,
            DWMWA_USE_IMMERSIVE_DARK_MODE,
            &theme,
            sizeof(BOOL)
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
