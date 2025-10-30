//
// Created by TX on 2025/8/6.
//

#ifndef QWIDGET_FANCYUI_SYSTEMACCESSOR_H
#define QWIDGET_FANCYUI_SYSTEMACCESSOR_H

#include <QColor>
#include <QMap>
#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <dwmapi.h>
#include <windows.h>
#endif

namespace fancy
{
    enum class SysAccentPalette;
    struct WINDOWCOMPOSITIONATTRIBDATA;
    struct WindowsVersion;

    class SystemAccessor
    {
    public:
        /**
         * 从 Windows 8 开始，由于窗口的呈现方式发生了样式更改，因此调用此函数不会产生模糊效果，而是单纯的透明效果。
         * @param hWnd 需要设置的窗口句柄
         * @param pBlurBehind 指向 DWM_BLURBEHIND 结构体对象的指针
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool DwmEnableBlurBehindWindow(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind);

        /**
         * 控制窗口的强调色、模糊/亚克力背景特效、圆角等，用于Win10和早期Win11版本，不推荐在Win11中使用
         * @param hwnd 需要设置的窗口句柄
         * @param data 指向WINDOWCOMPOSITIONATTRIBDATA结构体对象的指针
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool SetWindowCompositionAttribute(HWND hwnd, WINDOWCOMPOSITIONATTRIBDATA *data);

        /**
         * 控制着窗口的视觉和行为，如 Mica/Acrylic、标题栏/边框颜色定制、窗口圆角、窗口动画等
         * @param hwnd 要为其设置属性值的窗口的句柄
         * @param dwAttribute DWMWINDOWATTRIBUTE 枚举值，此参数指定要设置的属性，pvAttribute参数指向包含该属性值的对象
         * @param pvAttribute 指向包含要设置的属性值的对象的指针。值集的类型取决于 dwAttribute 参数的值。DWMWINDOWATTRIBUTE 枚举主题在每个标志的行中指示应在 pvAttribute 参数中传递指针的值类型
         * @param cbAttribute 通过 pvAttribute 参数设置的属性值的大小（以字节为单位）。值集的类型及其大小（以字节为单位）取决于 dwAttribute 参数的值。
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool DwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);

        /**
         * 将窗口框架/非客户区扩展到工作区/客户区。
         * @param hWnd 框架将扩展到工作区的窗口的句柄。
         * @param pMarInset 指向 MARGINS 结构的指针，该结构描述在将帧扩展到工作区时要使用的边距。
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool DwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS *pMarInset);

        /**
         * 检索桌面窗口管理器 (DWM) 玻璃合成使用的当前颜色。 此值基于当前配色方案，可由用户修改。
         * 注意！该颜色值是经过系统校正和透明度处理后的结果，它不是个性化设置中的颜色本身，而是一个用于窗口边框/任务栏等透明区域渲染的混合色
         * 应用程序可以通过处理 WM_DWMCOLORIZATIONCOLORCHANGED 通知来侦听颜色更改。
         * @param pcrColorization 指向一个值的指针，当此函数成功返回时，该值接收用于玻璃合成的当前颜色。 颜色格式为 0xAARRGGBB
         * @param pfOpaqueBlend 指向值的指针，当此函数成功返回时，该值指示颜色是否为不透明混合。 如果颜色不透明混合，则为 TRUE;否则为 FALSE。
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool DwmGetColorizationColor(DWORD *pcrColorization, BOOL *pfOpaqueBlend);

        /**
         * 获取系统主题色及其衍生色
         * @return QMap，返回SysAccentPalette枚举（与WinRt中的枚举名称对应）与颜色值的键值对
         * 颜色存储时的顺序为由浅到深，中间值为系统个性化设置中显示的颜色
         * 若函数调用失败则打印信息并返回空的 QMap
         */
        static QMap<SysAccentPalette, QColor> sysAccentColors();

        /**
         * 更改子窗口、弹出窗口或顶级窗口的大小、位置和 Z 顺序。
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);

        /**
         * 使应用程序能够保护自己的屏幕窗口内容，使其免受通过一组特定的公共操作系统功能和 API 捕获或复制。
         * 仅当桌面窗口管理器 (DWM) 组合桌面时，它才有效
         * @param hWnd 顶级窗口的句柄。 窗口必须属于当前进程
         * @param dwAffinity 指定窗口内容的显示位置的显示相关性设置
         * @return 如果函数成功，则返回true,否则返回false
         */
        static bool SetWindowDisplayAffinity(HWND hWnd, DWORD dwAffinity);

        /**
         * 获取Windows系统版本号信息，使用WindowsVersion结构体保存
         * @return 静态WindowsVersion结构体变量的引用
         */
        static const WindowsVersion &windowsVersion(); //获取Windows版本信息 //单例模式,首次初始化

        /**
         * @return 是否是Windows11操作系统 若是则返回true否则返回false
         */
        static bool isWindows11();

        /**
         * @return 是否是Windows10操作系统 若是则返回true否则返回false
         */
        static bool isWindows10();

        /**
         * @return 是否是Windows10以下的操作系统 若是则返回true否则返回false
         */
        static bool isBelowWindows10();

        ~SystemAccessor();

    private:
        static HMODULE _user32dll;
        static HMODULE _ntdll;
    };
} // fancy

#endif //QWIDGET_FANCYUI_SYSTEMACCESSOR_H
