#ifndef DWMWINDOWEFFECT_H
#define DWMWINDOWEFFECT_H

#include "windowmanager.h"

//设置非客户区效果，默认拓展非客户区到整个窗口
class DwmWindowEffect : protected WindowManager
{
public:
    static bool setMicaEffect(const HWND hWnd); //云母

    static bool setMicaAltEffect(const HWND hWnd); //云母Alt

    static bool setAcrylicEffect(const HWND hWnd);//亚克力

    static bool setTitleBarColor(const HWND hWnd,COLORREF rgb);//设置标题栏颜色(不支持透明度)

    static bool setHeaderTextColor(const HWND hWnd, COLORREF rgb);//设置标题文字颜色//使用RGB宏生成颜色//恢复系统默认的颜色：DEFAULT_COLOR

    static bool setBorderColor(const HWND hWnd, COLORREF rgb);//设置边框颜色//颜色参数设置：TRANSPARENT_COLOR宏可使边框隐藏，DEFAULT_COLOR可恢复默认边框

    static bool disabledNonClientRendering(const HWND hWnd);//禁用窗口非客户区(边框)渲染,忽略窗口样式//(win7风格简陋窗口)

    static bool enabledNonClientRendering(const HWND hWnd);//启用窗口非客户区(边框)渲染,忽略窗口样式//(恢复)

    static bool setWindowDarkMode(const HWND hWnd);//设置窗口深色模式

    static bool setWindowLightMode(const HWND hWnd);//设置窗口浅色模式

    static bool setEntireWindowFullyTransparent(const HWND hWnd);//整个窗口全透明(win8以下是模糊窗口效果)
};

#endif // DWMWINDOWEFFECT_H
