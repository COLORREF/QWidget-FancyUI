#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H
#define ABGR(a, b, g, r) ((COLORREF)(((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)))
#define TRANSPARENT_COLOR 0xFFFFFFFE //仅对边框生效
#define DEFAULT_COLOR 0xFFFFFFFF
#include <Windows.h>
#include <WinUser.h>
#include <dwmapi.h>

class WindowManager
{
public:
    static LRESULT moveWindow(const HWND hwnd);//调用WinAPI移动窗口

    static bool disableMaximize(const HWND hWnd);//禁用最大化

    static bool disableMinimize(const HWND hWnd);//禁用最小化

    static bool disablesResizingViaBorders(const HWND hWnd);//禁止通过边框调整窗口大小

    static bool disableTitleBar(const HWND hWnd);//禁用标题栏

    static bool setWindowTop(const HWND hWnd);//窗口置顶

    static bool cancelWindowTop(const HWND hWnd);//取消窗口置顶

    static bool allowWindowCaptured(const HWND hWnd);//允许窗口捕获

    static bool prohibitWindowCaptured(const HWND hWnd);//禁止窗口捕获

protected:

    enum class WINDOWCOMPOSITIONATTRIB
    {
        WCA_UNDEFINED,
        WCA_NCRENDERING_ENABLED,
        WCA_NCRENDERING_POLICY ,
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
        ACCENT_DISABLED,//黑色无效果
        ACCENT_ENABLE_GRADIENT,//直接设置颜色
        ACCENT_ENABLE_TRANSPARENTGRADIENT,//跟随系统主题色(需要窗口透明)
        ACCENT_ENABLE_AERO_BLUR, //Aero模糊
        ACCENT_ENABLE_AERO_BLUR_COLOR, //Aero模糊,但可直接设置颜色
        ACCENT_ENABLE_HOSTBACKDROP,//黑色无效果
        ACCENT_INVALID_STATE//全透明
    };

    struct ACCENT_POLICY
    {
        ACCENT_STATE AccentState; //指定窗口的效果
        DWORD AccentFlags;//可以用于控制无边框时的窗口阴影(参考值：480 影响因素未知)
        COLORREF GradientColor; //控制透明效果的颜色，取决于AccentState
        DWORD AnimationId;//未知作用，可能作用：控制动画效果
    };

    struct WINDOWCOMPOSITIONATTRIBDATA
    {
        WINDOWCOMPOSITIONATTRIB Attribute;
        ACCENT_POLICY* Data;
        SIZE_T SizeOfData;
    };

    static BOOL SetWindowCompositionAttribute(HWND hWnd, WINDOWCOMPOSITIONATTRIBDATA* data);

    static bool DwmSetWindowAttribute_(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);

    static bool DwmExtendFrameIntoClientArea_(HWND hWnd, const MARGINS* pMarInset);

    static LRESULT SendMessage_(HWND hWnd,UINT Msg, WPARAM wParam, LPARAM lParam);

    static bool DwmEnableBlurBehindWindow_(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);

    static LONG_PTR SetWindowLongPtr_(HWND hWnd,int nIndex,LONG_PTR dwNewLong);

    static LONG_PTR GetWindowLongPtr_(HWND hWnd,int nIndex);

    static bool SetWindowPos_(HWND hWnd,HWND hWndInsertAfter,int X,int Y,int cx, int cy,UINT uFlags);

    static bool SetWindowDisplayAffinity_(HWND hWnd, DWORD dwAffinity);

private:
    static BOOL(WINAPI* SetWindowCompositionAttributePtr)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

    WindowManager();
};


#endif // WINDOWMANAGER_H


