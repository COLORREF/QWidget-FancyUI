#ifndef WINDOWEFFECT_H
#define WINDOWEFFECT_H
#include <Windows.h>
#include <WinUser.h>
#include <dwmapi.h>

#define ABGR(a, b, g, r) ((COLORREF)(((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)))

//大部分功能均需设置窗口透明
class WindowEffect
{
private:

    enum class WINDOWCOMPOSITIONATTRIB
    {
        WCA_UNDEFINED = 0,
        WCA_NCRENDERING_ENABLED = 1,
        WCA_NCRENDERING_POLICY = 2,
        WCA_TRANSITIONS_FORCEDISABLED = 3,
        WCA_ALLOW_NCPAINT = 4,
        WCA_CAPTION_BUTTON_BOUNDS = 5,
        WCA_NONCLIENT_RTL_LAYOUT = 6,
        WCA_FORCE_ICONIC_REPRESENTATION = 7,
        WCA_EXTENDED_FRAME_BOUNDS = 8,
        WCA_HAS_ICONIC_BITMAP = 9,
        WCA_THEME_ATTRIBUTES = 10,
        WCA_NCRENDERING_EXILED = 11,
        WCA_NCADORNMENTINFO = 12,
        WCA_EXCLUDED_FROM_LIVEPREVIEW = 13,
        WCA_VIDEO_OVERLAY_ACTIVE = 14,
        WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
        WCA_DISALLOW_PEEK = 16,
        WCA_CLOAK = 17,
        WCA_CLOAKED = 18,
        WCA_ACCENT_POLICY = 19,
        WCA_FREEZE_REPRESENTATION = 20,
        WCA_EVER_UNCLOAKED = 21,
        WCA_VISUAL_OWNER = 22,
        WCA_HOLOGRAPHIC = 23,
        WCA_EXCLUDED_FROM_DDA = 24,
        WCA_PASSIVEUPDATEMODE = 25,
        WCA_USEDARKMODECOLORS = 26,
        WCA_CORNER_STYLE = 27,
        WCA_PART_COLOR = 28,
        WCA_DISABLE_MOVESIZE_FEEDBACK = 29,
        WCA_LAST = 30
    };

    enum class ACCENT_STATE
    {
        ACCENT_DISABLED = 0,//黑色无效果
        ACCENT_ENABLE_GRADIENT = 1,//直接设置颜色
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,//跟随系统主题色(需要窗口透明)
        ACCENT_ENABLE_AERO_BLUR = 3, //Aero模糊
        ACCENT_ENABLE_AERO_BLUR_COLOR = 4, //Aero模糊,但可直接设置颜色
        ACCENT_ENABLE_HOSTBACKDROP = 5,//黑色无效果
        ACCENT_INVALID_STATE = 6//全透明
    };

    struct ACCENT_POLICY
    {
        ACCENT_STATE AccentState; //指定窗口的透明效果
        DWORD AccentFlags = 0;//用于控制透明效果的行为(测试下来没好像啥用)
        COLORREF GradientColor; //透明效果的颜色
        DWORD AnimationId = 0;//用于标识透明效果的动画效果
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

    static bool setTitleBarColor(const HWND hwnd,int red, int green, int blue);//设置标题栏颜色(不支持透明度)

    //以下函数,将窗口设置透明时生效

    static void setFollowingSystemColor(const HWND hwnd);//跟随系统颜色

    static void setAreoEffect(const HWND hwnd); //Areo

    static void setAreoEffect(const HWND hwnd,COLORREF abgr);//Areo,16进制颜色,每两位表示一个像素分量

    static void setFullTransparency(const HWND hwnd);//全透明

    static void setMicaEffect(const HWND hwnd); //云母

    static void setMicaAltEffect(const HWND hwnd); //云母Alt

    static void setAcrylicEffect(const HWND hwnd);//亚克力

    //下面两个函数在未使用上方的函数设置窗口效果时，仅标题栏生效

    static bool setWindowDarkMode(const HWND hwnd);//设置窗口深色模式

    static bool setWindowLightMode(const HWND hwnd);//设置窗口浅色模式

    //其余功能封装中......

protected:
    WindowEffect();
};


#endif // WINDOWEFFECT_H
