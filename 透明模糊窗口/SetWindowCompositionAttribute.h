#ifndef SETWINDOWCOMPOSITIONATTRIBUTE_H
#define SETWINDOWCOMPOSITIONATTRIBUTE_H

#include <Windows.h>

//在Windows上，有一些函数是在运行时动态加载的，而不是在编译时静态链接。
//SetWindowCompositionAttribute函数就是其中之一，函数实现在user32.dll中
//因此无法通过链接user32.lib直接调用

//为了调用 Windows 的 API，需要定义一些结构体和枚举类

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
    WCA_LAST = 23
};

struct WINDOWCOMPOSITIONATTRIBDATA
{
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID pvData;
    SIZE_T cbData;
};

//客户区状态枚举
enum class ACCENT_STATE
{
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND = 3, //Aero效果
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4, // 亚克力效果
    ACCENT_INVALID_STATE = 5
};

//设置客户区的具体属性
struct ACCENT_POLICY
{
    ACCENT_STATE AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
};

//与目标函数类型相匹配的函数指针类型，名称为function
typedef BOOL(WINAPI* function)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);//WINAPI可以删去，但不建议


HMODULE huser = GetModuleHandleW(L"user32.dll");//获取user32.dll模块句柄

//从dll中检索SetWindowCompositionAttribute函数，返回一个地址，强转成对应的函数指针
//SetWindowCompositionAttribute就是需要用的函数(可调用对象)
function SetWindowCompositionAttribute = (function)GetProcAddress(huser, "SetWindowCompositionAttribute");

#endif // SETWINDOWCOMPOSITIONATTRIBUTE_H
