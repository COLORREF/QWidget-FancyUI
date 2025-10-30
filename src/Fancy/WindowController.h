//
// Created by TX on 2025/8/6.
//

#ifndef QWIDGET_FANCYUI_WINDOWCOMPOSITOR_H
#define QWIDGET_FANCYUI_WINDOWCOMPOSITOR_H
#include "SystemAccessor.h"

namespace fancy
{
    enum class Theme;
    enum class DwmWindowEffect;
    enum class CompositionWindowEffect;

    class WindowController
    {
    public:
        /**
         * 调用WinAPI移动窗口，在鼠标按下事件中使用
         */
        static LRESULT moveWindow(HWND hWnd);

        /**
         * 禁用原生标题栏中的最大化按钮，同时移除最大化动画，但窗口仍然可以最大化
         * @param hWnd 窗口句柄
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool disableMaximize(HWND hWnd);

        /**
         * 禁用原生标题栏中的最小化按钮，同时移除最小化动画，但窗口仍然可以最小化
         * @param hWnd 窗口句柄
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool disableMinimize(HWND hWnd);

        /**
         * 禁止通过边框调整窗口大小，窗口会无法获取到通过边框调整大小的消息
         * @param hWnd 窗口句柄
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool disablesResizingViaBorders(HWND hWnd);

        /**
         * 禁用窗口原生标题栏，告知系统此窗口为无标题栏窗口，调用此函数将移除窗口动画
         * 此函数会在窗口顶部留下约4像素宽无法消除的边框
         * 无边框窗口的实现不依赖此函数
         * @param hWnd 窗口句柄
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool disableTitleBar(HWND hWnd);

        /**
         * 设置窗口是否置顶
         * @param hWnd 窗口句柄
         * @param top true使得窗口置顶，false取消窗口置顶
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setWindowTop(HWND hWnd, bool top);

        /**
         * 调用 SetWindowCompositionAttribute 函数设置窗口工作区的视觉效果。
         * 当 CompositionWindowEffect 为以下值时，abgr参数无效，将被忽略。
         * Disabled、SystemColor、Areo、FullyTransparent
         * @param hWnd 窗口句柄
         * @param effect 需要设置的窗口效果类型，CompositionWindowEffect 枚举
         * @param abgr ABGR排序的颜色值，使用ABGR宏生成
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setCompositionWindowEffect(HWND hWnd, CompositionWindowEffect effect, COLORREF abgr = 0);


        /**
         * 设置窗口非工作区的DWM渲染效果，此函数不会主动拓展非工作区到工作区
         * 若要拓展非工作区到工作区请调用extendFrameIntoClientArea函数
         * @param hWnd 窗口句柄
         * @param effect 需要设置的窗口效果类型，DwmWindowEffect 枚举
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setDwmWindowEffect(HWND hWnd, DwmWindowEffect effect);

        /**
         * 将窗口框架（非工作区）扩展到工作区，默认参数会将窗口框架拓展到整个工作区
         * @param hWnd 窗口句柄
         * @param margins 该结构描述在将框架扩展到工作区时要使用的边距
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool extendFrameIntoClientArea(HWND hWnd, MARGINS margins = {-1});

        /**
         * 设置非工作区DWM呈现（渲染）策略
         * @param hWnd 需要设置非客户区呈现策略的窗口句柄
         * @param enable false则禁用，true则启用
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setNonClientDwmRenderingStrategy(HWND hWnd, bool enable);

        /**
         * 设置窗口边框的颜色，若系统开启了“在标题栏和窗口边框上显示强调色”选项（个性化设置中），则窗口边框颜色默认为系统色
         * rgb参数不支持透明度，调用此函数后，修改系统色不会导致窗口边框颜色同步变化
         * 将rgb值置为DWMWA_COLOR_NONE宏可在视觉上隐藏边框
         * 将rgb值置为DWMWA_COLOR_DEFAULT宏可恢复默认的边框颜色，并跟随系统变化
         * @param hWnd 窗口句柄
         * @param rgb 要设置的边框颜色值，使用RGB宏生成
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setBorderColor(HWND hWnd, COLORREF rgb);

        /**
         * 设置窗口标题栏文字的颜色，仅对原生标题栏生效
         * rgb参数不支持透明度
         * 若要恢复系统默认的颜色将rgb参数值置为DWMWA_COLOR_DEFAULT即可
         * rgb参数值置为DWMWA_COLOR_NONE宏时函数调用失败
         * @param hWnd 窗口句柄
         * @param rgb 要设置的标题栏文字的颜色值，使用RGB宏生成
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setTitleTextColor(HWND hWnd, COLORREF rgb);

        /**
         * 设置标题栏颜色，若系统开启了“在标题栏和窗口边框上显示强调色”选项（个性化设置中），则标题栏颜色默认为系统色
         * rgb参数不支持透明度，调用此函数后，修改系统色不会导致标题栏颜色同步变化
         * 将rgb值置为DWMWA_COLOR_DEFAULT宏可恢复默认的标题栏颜色，并跟随系统变化
         * 将rgb值置为DWMWA_COLOR_NONE宏效果与DWMWA_COLOR_DEFAULT一样
         * @param hWnd 窗口句柄
         * @param rgb 要设置的标题栏的颜色值，使用RGB宏生成
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setTitleBarColor(HWND hWnd, COLORREF rgb);

        /**
         * 指示窗口非客户区以何种主题模式呈现（深色/浅色），会改变窗口阴影、标题栏颜色等视觉效果的呈现
         * 若系统开启了“在标题栏和窗口边框上显示强调色”选项（个性化设置中），或调用了 setTitleBarColor 设置了标题栏颜色
         * 则标题栏颜色默认为系统色/设置色，调用此函数不会使标题栏颜色变化
         * 函数的调用是一次性的，此后，窗口的主题模式（呈现效果）会跟随系统的主题模式改变
         * @param hWnd 窗口句柄
         * @param theme Theme::Dark指示窗口以深色模式呈现，Theme::Light指示窗口以浅色模式呈现
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setWindowThemeMode(HWND hWnd, Theme theme);

        /**
         * 设置窗口的捕获策略，即是否能被录频/截图软件或相关API获取到窗口图像
         * @param hWnd 顶级窗口的句柄，窗口必须属于当前进程
         * @param capture true允许捕获，false禁止捕获
         * @param hide true将会使窗口被捕获时完全隐藏，false则使窗口呈现为纯黑色
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool setWindowCaptureStrategy(HWND hWnd, bool capture, bool hide = true);

        /**
         * 从 Windows 8 开始，由于窗口的呈现方式发生了样式更改，因此调用此函数不会产生模糊效果，而是单纯的透明效果。
         * 默认情况下窗口的工作区会呈现一种类似带有黑色遮罩的透明色，并非完全透明，非工作区无变化
         * 即使修改内部调用函数 DwmEnableBlurBehindWindow 的参数细节仍无变化
         * 使用 extendFrameIntoClientArea 将窗口非工作区扩展到整个工作区，可以得到完全透明的窗口（包括工作区和非工作区）
         * setCompositionWindowEffect 设置的全透明效果仅作用于客户区，注意区别
         * 此函数不会主动拓展非工作区
         * @param hWnd 窗口句柄
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool enableBlurBehindWindow(HWND hWnd);
    };
} // fancy

#endif //QWIDGET_FANCYUI_WINDOWCOMPOSITOR_H
