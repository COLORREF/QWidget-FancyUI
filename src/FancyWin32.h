#ifndef FANCYWIN32_H
#define FANCYWIN32_H
#ifdef _WIN32

#define ABGR(a, b, g, r) ((COLORREF)(((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)))
#define TRANSPARENT_COLOR 0xFFFFFFFE // 仅对边框生效
#define DEFAULT_COLOR 0xFFFFFFFF
#include <Windows.h>
#include <WinUser.h>
#include <dwmapi.h>
#include <winnt.h>
using RtlGetVersionPtr = NTSTATUS (NTAPI*)(PRTL_OSVERSIONINFOW);

struct WindowsVersion
{
    LONGLONG MajorVersion = -1;//主版本号
    LONGLONG MinorVersion = -1;//次版本号
    LONGLONG BuildNumber = -1;//构建版本号
};

class WindowManager
{
public:
    static LRESULT moveWindow(const HWND hwnd); // 调用WinAPI移动窗口

    static bool disableMaximize(const HWND hWnd); // 禁用最大化，仅对原生窗口生效

    static bool disableMinimize(const HWND hWnd); // 禁用最小化，仅对原生窗口生效

    static bool disablesResizingViaBorders(const HWND hWnd); // 禁止通过边框调整窗口大小，仅对原生窗口生效

    static bool disableTitleBar(const HWND hWnd); // 禁用标题栏，仅对原生窗口生效，对无边框窗口调用此函数将移除窗口动画

    static bool setWindowTop(const HWND hWnd); // 窗口置顶

    static bool cancelWindowTop(const HWND hWnd); // 取消窗口置顶

    static bool allowWindowCaptured(const HWND hWnd); // 允许窗口捕获

    static bool prohibitWindowCaptured(const HWND hWnd); // 禁止窗口捕获

    static const WindowsVersion &windowsVersion();//获取Windows版本信息 //单例模式,首次初始化

    static bool isWindows11();//是否是 Windows 11操作系统

    static bool isWindows10();//是否是 Windows 10操作系统

    static bool isWindowsVersionBelowWindows10();//是否是 Windows 10以下操作系统

    ~WindowManager();

protected:
    enum class WINDOWCOMPOSITIONATTRIB
    {
        WCA_UNDEFINED,
        WCA_NCRENDERING_ENABLED,
        WCA_NCRENDERING_POLICY,
        WCA_TRANSITIONS_FORCEDISABLED,
        WCA_ALLOW_NCPAINT,
        WCA_CAPTION_BUTTON_BOUNDS,
        WCA_NONCLIENT_RTL_LAYOUT,
        WCA_FORCE_ICONIC_REPRESENTATION,
        WCA_EXTENDED_FRAME_BOUNDS,
        WCA_HAS_ICONIC_BITMAP,
        WCA_THEME_ATTRIBUTES,
        WCA_NCRENDERING_EXILED,
        WCA_NCADORNMENTINFO,
        WCA_EXCLUDED_FROM_LIVEPREVIEW,
        WCA_VIDEO_OVERLAY_ACTIVE,
        WCA_FORCE_ACTIVEWINDOW_APPEARANCE,
        WCA_DISALLOW_PEEK,
        WCA_CLOAK,
        WCA_CLOAKED,
        WCA_ACCENT_POLICY,
        WCA_FREEZE_REPRESENTATION,
        WCA_EVER_UNCLOAKED,
        WCA_VISUAL_OWNER,
        WCA_HOLOGRAPHIC,
        WCA_EXCLUDED_FROM_DDA,
        WCA_PASSIVEUPDATEMODE,
        WCA_USEDARKMODECOLORS,
        WCA_CORNER_STYLE,
        WCA_PART_COLOR,
        WCA_DISABLE_MOVESIZE_FEEDBACK,
        WCA_LAST
    };

    enum class ACCENT_STATE
    {
        ACCENT_DISABLED,                   // 黑色无效果
        ACCENT_ENABLE_GRADIENT,            // 直接设置颜色
        ACCENT_ENABLE_TRANSPARENTGRADIENT, // 跟随系统主题色(需要窗口透明)
        ACCENT_ENABLE_AERO_BLUR,           // Aero模糊
        ACCENT_ENABLE_AERO_BLUR_COLOR,     // Aero模糊,但可直接设置颜色
        ACCENT_ENABLE_HOSTBACKDROP,        // 黑色无效果
        ACCENT_INVALID_STATE               // 全透明
    };

    struct ACCENT_POLICY
    {
        ACCENT_STATE AccentState; // 指定窗口的效果
        DWORD AccentFlags;        // 可以用于控制无边框时的窗口阴影(参考值：480 影响因素未知)
        COLORREF GradientColor;   // 控制透明效果的颜色，取决于AccentState
        DWORD AnimationId;        // 未知作用，可能作用：控制动画效果
    };

    struct WINDOWCOMPOSITIONATTRIBDATA
    {
        WINDOWCOMPOSITIONATTRIB Attribute;
        ACCENT_POLICY *Data;
        SIZE_T SizeOfData;
    };

    static BOOL SetWindowCompositionAttribute(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA *data);

    static bool DwmSetWindowAttribute_(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);

    static bool DwmExtendFrameIntoClientArea_(HWND hWnd, const MARGINS *pMarInset);

    static LRESULT SendMessage_(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    static bool DwmEnableBlurBehindWindow_(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind);

    static LONG_PTR SetWindowLongPtr_(HWND hWnd, int nIndex, LONG_PTR dwNewLong);

    static LONG_PTR GetWindowLongPtr_(HWND hWnd, int nIndex);

    static bool SetWindowPos_(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

    static bool SetWindowDisplayAffinity_(HWND hWnd, DWORD dwAffinity);

private:

    static HMODULE user32dll;

    static HMODULE ntdll;

    static BOOL(WINAPI *SetWindowCompositionAttributePtr)(HWND, WINDOWCOMPOSITIONATTRIBDATA *);

    static RtlGetVersionPtr RtlGetVersion;

    WindowManager();
};

class CompositionWindowEffect : protected WindowManager
{
public:
    static void removeCompositionWindowEffect(const HWND hWnd); // 移除下方的函数设置的窗口效果

    static void setFollowingSystemColor(const HWND hwnd); // 跟随系统颜色

    static void setAreoEffect(const HWND hwnd); // Areo

    static void setAreoEffect(const HWND hwnd, COLORREF abgr); // Areo,16进制颜色,每两位表示一个像素分量//使用ABGR宏生成颜色

    static void setClientAreaFullyTransparent(const HWND hwnd); // 客户区全透明
};

// 设置非客户区效果，默认拓展非客户区到整个窗口
class DwmWindowEffect : protected WindowManager
{
public:
    static bool setMicaEffect(const HWND hWnd); // 云母

    static bool setMicaAltEffect(const HWND hWnd); // 云母Alt

    static bool setAcrylicEffect(const HWND hWnd); // 亚克力

    static bool setTitleBarColor(const HWND hWnd, COLORREF rgb); // 设置标题栏颜色(不支持透明度)

    static bool setHeaderTextColor(const HWND hWnd, COLORREF rgb); // 设置标题文字颜色//使用RGB宏生成颜色//恢复系统默认的颜色：DEFAULT_COLOR//仅对原生窗口生效

    static bool setBorderColor(const HWND hWnd, COLORREF rgb); // 设置边框颜色//颜色参数设置：TRANSPARENT_COLOR宏可使边框隐藏，DEFAULT_COLOR可恢复默认边框

    static bool disabledNonClientRendering(const HWND hWnd); // 禁用窗口非客户区(边框)渲染,忽略窗口样式//(win7风格简陋窗口)

    static bool enabledNonClientRendering(const HWND hWnd); // 启用窗口非客户区(边框)渲染,忽略窗口样式//(恢复)

    static bool setWindowDarkMode(const HWND hWnd); // 设置窗口深色模式

    static bool setWindowLightMode(const HWND hWnd); // 设置窗口浅色模式

    static bool setEntireWindowFullyTransparent(const HWND hWnd); // 整个窗口全透明(win8以下是模糊窗口效果)
};

#endif
#endif // FANCYWIN32_H
