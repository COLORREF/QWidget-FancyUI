#include "compositionwindoweffect.h"

void CompositionWindowEffect::removeCompositionWindowEffect(const HWND hWnd)
{
    ACCENT_POLICY accent{ACCENT_STATE::ACCENT_DISABLED,0,0,0};
    WINDOWCOMPOSITIONATTRIBDATA data{WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hWnd, &data);
}

void CompositionWindowEffect::setFollowingSystemColor(const HWND hwnd)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_ENABLE_TRANSPARENTGRADIENT, 0, 0, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}

void CompositionWindowEffect::setAreoEffect(const HWND hwnd)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_ENABLE_AERO_BLUR, 0, 0, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}

void CompositionWindowEffect::setAreoEffect(const HWND hwnd, COLORREF abgr)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_ENABLE_AERO_BLUR_COLOR, 0, abgr, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}

void CompositionWindowEffect::setClientAreaFullyTransparent(const HWND hwnd)
{
    ACCENT_POLICY accent = { ACCENT_STATE::ACCENT_INVALID_STATE, 0, 0, 0 };
    WINDOWCOMPOSITIONATTRIBDATA data{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent,sizeof(ACCENT_POLICY) };
    SetWindowCompositionAttribute(hwnd, &data);
}
