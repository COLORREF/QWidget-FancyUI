#ifndef WINDOWEFFECT_H
#define WINDOWEFFECT_H
#include <Windows.h>
#include <WinUser.h>
#include <dwmapi.h>

#define ABGR(a, b, g, r) ((COLORREF)(((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)))
#define TRANSPARENT_COLOR 0xFFFFFFFE //仅对边框生效
#define DEFAULT_COLOR 0xFFFFFFFF

//大部分功能均需设置窗口透明
class WindowEffect
{
private:

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
        ACCENT_STATE AccentState; //指定窗口的透明效果
        DWORD AccentFlags;//用于控制透明效果的行为
        COLORREF GradientColor; //透明效果的颜色
        DWORD AnimationId;//用于标识透明效果的动画效果
    };

    struct WINDOWCOMPOSITIONATTRIBDATA
    {
        WINDOWCOMPOSITIONATTRIB Attribute;
        ACCENT_POLICY* Data;
        SIZE_T SizeOfData;
    };

    static BOOL(WINAPI* SetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

public:

    static void moveWindow(const HWND hwnd); //调用WinAPI移动窗口

    static bool setTitleBarColor(const HWND hwnd,COLORREF rgb);//设置标题栏颜色(不支持透明度)

    //以下函数,将窗口设置透明时生效

    static void setFollowingSystemColor(const HWND hwnd);//跟随系统颜色

    static void setAreoEffect(const HWND hwnd); //Areo

    static void setAreoEffect(const HWND hwnd,COLORREF abgr);//Areo,16进制颜色,每两位表示一个像素分量//使用ABGR生成颜色

    static void setFullTransparency(const HWND hwnd);//全透明

    static void setMicaEffect(const HWND hwnd); //云母

    static void setMicaAltEffect(const HWND hwnd); //云母Alt

    static void setAcrylicEffect(const HWND hwnd);//亚克力

    //下面前两个函数在未使用上方的函数设置窗口效果时，仅标题栏生效

    static bool setWindowDarkMode(const HWND hwnd);//设置窗口深色模式

    static bool setWindowLightMode(const HWND hwnd);//设置窗口浅色模式

    static bool setHeaderTextColor(const HWND hWnd, COLORREF rgb);//设置标题文字颜色//使用RGB宏生成颜色//恢复系统默认的颜色：DWMWA_COLOR_DEFAULT

    static bool setBorderColor(const HWND hWnd, COLORREF rgb);//设置边框颜色//颜色参数设置：TRANSPARENT_COLOR宏可使边框隐藏，DEFAULT_COLOR可恢复默认边框

    //其余功能封装中......

protected:
    WindowEffect();
};


#endif // WINDOWEFFECT_H
