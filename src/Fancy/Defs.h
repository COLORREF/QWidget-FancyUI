//
// Created by TX on 2025/8/8.
//

#ifndef QWIDGET_FANCYUI_ENUM_H
#define QWIDGET_FANCYUI_ENUM_H

#include <algorithm>
#include <QColor>
#include <QImage>
#include <QLine>
#include <QMetaObject>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QtMath>
#include <QtSystemDetection>
#include <QWidget>
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 3000
#include <magic_enum.hpp>

#include "IconEnums.h"

#define ABGR(a, b, g, r) ((COLORREF)(((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)))

#ifdef __GNUC__
#define DWMWA_COLOR_DEFAULT 0xFFFFFFFF
#define DWMWA_COLOR_NONE    0xFFFFFFFE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#define DWMWA_BORDER_COLOR 34
#define DWMWA_CAPTION_COLOR 35
#define DWMWA_TEXT_COLOR 36
#define DWMWA_SYSTEMBACKDROP_TYPE 38

enum DWM_SYSTEMBACKDROP_TYPE
{
    DWMSBT_AUTO,
    DWMSBT_NONE,
    DWMSBT_MAINWINDOW,
    DWMSBT_TRANSIENTWINDOW,
    DWMSBT_TABBEDWINDOW,
};
#endif

#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <dwmapi.h>
#include <windows.h>
#include <winnt.h>
#define THEME_REGEDIT_PATH TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize")
#define FULL_THEME_REGEDIT_PATH TEXT("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize")
#define THEME_REGEDIT_KEY TEXT("AppsUseLightTheme")
#define ACCENT_PALETTE_REGEDIT_PATH TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Accent")
#define FULL_ACCENT_PALETTE_REGEDIT_PATH TEXT("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Accent")
#define ACCENT_PALETTE_KEY TEXT("AccentPalette")
#endif

namespace fancy
{
    Q_NAMESPACE

    enum class Theme
    {
        Light,
        Dark
    };

    enum class ColorGroups
    {
        System, // 跟随系统的配色方案，默认情况下将使用此方案，仅用于部分突出强调颜色（如按钮背景色等），其余颜色由硬编码指定且三者均相同（如通用边框色等）
        Default, // 默认情况下的配色方案，当系统不支持查询主题色时将使用此方案，配色方案由本库的开发者@COLORREF提供
        Custom // 自定义的配色方案，由用户或开发者手动指定
    };

    enum class SysAccentPalette
    {
        // 此处枚举为系统主题色及其衍生色，命名采用与WinRT中的命名方式一致（顺便吐槽一下这个构式命名）
        // 值从小到大的顺序对应颜色由浅到深
        // ！不要修改顺序，sysAccentColors 实现函数依赖此顺序
        AccentLight3, // 最浅的主题色  场景：1、深色模式下鼠标悬浮在控件（如右下角面板打开后的WiFi按钮）上时使用的颜色 2、深色模式下系统超链接标签文字颜色
        AccentLight2, // 较浅的主题色  场景：1、深色模式下系统控件的默认颜色（如系统设置界面侧边栏指示器的颜色、单选框内部背景色）2、浅色模式下系统超链接标签文字颜色
        AccentLight1, // 浅色主题色
        Accent, // 个性化设置中实际设置显示的颜色
        AccentDark1, // 深色主题色   场景：1、浅色模式下系统控件的默认颜色
        AccentDark2, // 较深的主题色  场景：1、系统底部任务栏颜色（需要开启深色模式以及“在开始和任务栏显示重点颜色”）
        // 2、浅色模式下鼠标悬浮在控件（如右下角面板打开后的WiFi按钮）上时使用的颜色
        // 3、浅色模式下用于突出重要内容的超链接标签颜色（如任务管理器设置中相关超链接标签）
        AccentDark3, // 最深的主题色  场景：深色模式下系统列表控件选中时的颜色（如任务管理器中内存或cpu消耗占比较高的单元格颜色与之非常接近）
    };

    enum class ColorRole
    {
        Window, // 窗口背景色
        WindowText, // 窗口标题等窗口信息文本色
        WindowBright, // 窗口高度对比色（关闭按钮悬浮时基础色）
        Border, // 通用边框色
        Text, // 通用文本色
        Button, // 按钮背景色
        ButtonBorder, // 按钮边框色
        ButtonBottomLine, // 按钮底线色
        RadioButtonInnerCircle, // 单选按钮内圈颜色
        Link, // 超链接色
        ImportantLink, // 重要的超链接色
        LinkVisited, // 已访问超链接色
        DisEnabled, // 通用禁用色
        DisEnabledText, //禁用文本色
        AppAccent, // 当前应用使用的强调色
        SysAccent, // 系统个性化设置中的强调色
        AppAccentText, // 以AppAccent为背景色的合适的文本
        MaskNormal, // 默认透明遮罩色
        MaskHover, // 悬浮透明遮罩色
        MaskPressed, // 按下透明遮罩色
        MaskBrightPressed, // 高度对比按下透明遮罩色（关闭按钮）
        MaskSelected, // 选中透明遮罩色
        MaskSelectedHover, // 选中悬浮遮罩色
        MaskSelectedPressed, // 选中按下遮罩色
        MSWindow, // MS窗口主区域背景色
        MSWindowSplitLine, // MS窗口分割线颜色
        ScrollBarNormal, // 滚动条普通背景色
        ScrollBarFocus, // 滚动条焦点背景色
        ScrollBarEndArrow, // 滚动条末端箭头颜色
        ScrollBarSlider, // 滚动条滑块颜色
        TextEditPanelNormal, // 文本编辑框面板普通色
        TextEditPanelFocus, // 文本编辑框面板焦点色
        TextEditPanelHover, //  文本编辑框面板悬浮色
        TextEditBorder, // 文本编辑框边框色
        TextEditIndicatorLine, // 文本编辑框指示线通用色
        PlaceholderText, // 占位符文本通用色
        Last
    };

    /// 标题栏按钮交互状态
    enum class TitleBtnState
    {
        Normal,
        Hover,
        Pressed,
        PressedOut
    };

    /// 指示最大化按钮的行为
    enum class MaximizeBtnState
    {
        Normal,
        Maximize
    };

    /// 控件最终的可绘制状态
    enum class VisualState
    {
        Normal,
        Pressed, // 按下 （控件启用、鼠标在其上、鼠标按下）
        Hover, // 悬浮 （控件启用、鼠标在其上、鼠标未按下）
        Selected, // 已选中 （控件启用、选中、鼠标不在其上、鼠标未按下）
        SelectedHover, // 选中 + 悬浮 （控件启用、选中、鼠标在其上方，鼠标未按下）
        SelectedPressed, // 选中 + 按下 （控件启用、选中，鼠标在其上方，鼠标按下）
        Disabled, // 禁用
        SelectedDisabled, // 已选中 + 禁用 （控件禁用、选中）
        Focus, // 获取到焦点（适用于文本框等控件）
        FocusHover, // 获取到焦点 + 鼠标悬浮 （适用于文本框等控件）
        Last,
        UnKnown
    };

    /// 控件类型
    enum class ControlType
    {
        IndicatorCheckable, // 可选中具有指示器的控件
        Widget, // 不可选中的控件
        WidgetCheckable, // 可选中但无指示器的控件
        TextEdit, // 文本编辑类控件
        Last
    };

    /// 图标缓存策略
    enum class IconCachePolicy
    {
        NoCache, // 不缓存，每次切换实时解析
        PerInstance, // 独立缓存，随 QIcon 生命周期释放
        SharedCache, // 共享缓存，全局静态哈希表
    };

    enum class Direction
    {
        Up,
        Down,
        Left,
        Right,
        UpLeft,
        UpRight,
        DownLeft,
        DownRight,
        None,
    };

    Q_ENUM_NS(Theme)
    Q_ENUM_NS(ColorGroups)
    Q_ENUM_NS(SysAccentPalette)
    Q_ENUM_NS(ColorRole)
    Q_ENUM_NS(TitleBtnState)
    Q_ENUM_NS(MaximizeBtnState)
    Q_ENUM_NS(VisualState)
    Q_ENUM_NS(ControlType)
    Q_ENUM_NS(IconCachePolicy)
    Q_ENUM_NS(IconStyle)
    Q_ENUM_NS(AntDesignIcons)
    Q_ENUM_NS(BootstrapIcons)
    Q_ENUM_NS(IconPark)
    Q_ENUM_NS(Direction)
#ifdef Q_OS_WIN
    enum class WINDOWCOMPOSITIONATTRIB
    {
        /**
        * 控制窗口的强调色、模糊/Areo背景特效。通过设置 ACCENT_POLICY 结构体实现。
        * WINDOWCOMPOSITIONATTRIBDATA 结构的 pvData 成员指向 ACCENT_POLICY 结构
        * 可开启客户区的Areo模糊效果等
        */
        WCA_ACCENT_POLICY = 19,
        /**
         * 防止桌面复制 API 捕获窗口。
         * WINDOWCOMPOSITIONATTRIBDATA 结构的 pvData 成员指向 BOOL
         * 如果值为 TRUE，则不会捕获窗口，否则，窗口表现出正常行为
         */
        WCA_EXCLUDED_FROM_DDA = 24,

        /**
        * 告知系统该窗口请求使用深色模式标题栏颜色规则
        * WINDOWCOMPOSITIONATTRIBDATA 结构的 pvData 指向 BOOL
        * 若为 TRUE 则请求深色模式，否则，请求浅色模式。
        * 若系统开启了“在标题栏和窗口边框上显示强调色”则无效（个性化设置中）
        * 设置效果为单次，若之后系统更改了深浅模式，则会同步更改
        */
        WCA_USEDARKMODECOLORS = 26,

        /**
         * 控制窗口圆角样式（直角/圆角）
         * pvData 指向 DWM_WINDOW_CORNER_PREFERENCE 枚举值，该枚举值定义在 dwmapi.h 中
         * DWMWCP_DEFAULT - 默认系统圆角
         * DWMWCP_DONOTROUND - 禁用圆角
         * DWMWCP_ROUND - 强制圆角
         * DWMWCP_ROUNDSMALL - 小圆角
         */
        WCA_CORNER_STYLE = 27
    };

    enum class ACCENT_STATE
    {
        /**
         * 禁用/移除SetWindowCompositionAttribute函数设置的所有的窗口特效
         * ACCENT_POLICY中的其余值全为0即可
         * 其余值产生的效果未知
         */
        ACCENT_DISABLED = 0, // 禁用特效 (默认黑色无效果)
        /**
         * 对窗口颜色进行纯色填充，需要窗口透明
         * 无视ACCENT_POLICY 结构体的 AccentFlags 变量值
         * 填充颜色取决于 ACCENT_POLICY 结构体的 GradientColor 变量值
         * 无视 GradientColor 的透明度分量
         */
        ACCENT_ENABLE_GRADIENT = 1, // 纯色填充
        /**
         * 启用透明渐变，需要窗口透明
         * 具体效果却决于 ACCENT_POLICY 结构体的 AccentFlags 变量值
         * 若 AccentFlags 为 0 则以系统主题色进行不透明纯色填充
         * 若 AccentFlags 为 2 则以 ACCENT_POLICY 中 GradientColor 的值进行具有透明度的填充
         * 其余值的具体效果未知，可能与 AccentFlags 等于 0 的效果相同
         */
        ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
        /**
         * Win7/Vista风格毛玻璃模糊（Areo模糊），需要窗口透明
         * 将 ACCENT_POLICY 结构体的 AccentFlags 变量值置为 0 以达到无色透明模糊效果
         * AccentFlags 为其余值时，会导致未知或不可控行为
         */
        ACCENT_ENABLE_BLURBEHIND = 3,
        /**
         * 带有颜色遮罩的Win7/Vista风格毛玻璃模糊（Areo模糊），需要窗口透明
         * 模糊颜色取决于 ACCENT_POLICY 中 GradientColor 的值
         * 无视 ACCENT_POLICY 中 AccentFlags 的值，GradientColor 颜色总会生效
         */
        ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,

        /**
         * 参数/效果未知
         *  ？(Win10 RS4+ 1709/1803+) 用于XAML Island？
         *  测试为黑色无效果
         */
        ACCENT_ENABLE_HOSTBACKDROP = 5,

        /**
         * 使用此枚举值会使窗口始终为全透明，需要窗口透明
         * 无视 ACCENT_POLICY 的其余参数
         */
        ACCENT_INVALID_STATE = 6 // 全透明
    };

    enum class CompositionWindowEffect
    {
        Disabled = 0, //禁用
        SolidColor = 1, //纯色填充
        SystemColor = 2, //系统色填充
        TransparentColor = 3, //透明色填充
        Areo = 4, //普通的Areo模糊
        AreoWithColor = 5, //带有颜色的Areo模糊
        FullyTransparent = 6 //全透明
    };

    enum class DwmWindowEffect
    {
        Acrylic = 0, // 亚克力
        Mica = 1, // 云母
        MicaAlt = 2, // 云母ALT
        Auto = 3, // 绘制默认背景，由操作系统决定
        None = 4, // 不要绘制背景，由操作系统决定
        Last // 仅作为初始化标记值，不要使用
    };

    struct ACCENT_POLICY
    {
        ACCENT_STATE AccentState; // 指定窗口的效果类型
        DWORD AccentFlags; // 似乎可以控制 GradientColor 生效与否（0不生效，参考值2，但似乎又与 AccentState 的值相关）
        COLORREF GradientColor; // ABRG颜色值，控制效果的颜色
        DWORD AnimationId; // ？保留字段/未公开动画ID？（可能作用：控制动画效果）
    };

    struct WINDOWCOMPOSITIONATTRIBDATA
    {
        WINDOWCOMPOSITIONATTRIB Attrib;
        void *pvData;
        SIZE_T cbData; //pvData 指向数据的大小 (sizeof(*pvData))
    };

    struct WindowsVersion
    {
        LONGLONG MajorVersion = -1; //主版本号
        LONGLONG MinorVersion = -1; //次版本号
        LONGLONG BuildNumber = -1; //构建版本号
    };
#endif

    struct IconId
    {
        IconStyle style;
        quint64 value;

        // default
        IconId() :
            style(IconStyle::AntDesignIcons),
            value(0) {}

        IconId(const IconStyle style, const quint64 value) :
            style(style),
            value(value) {}

        bool operator==(const IconId &other) const
        {
            return style == other.style && value == other.value;
        }
    };

    inline size_t qHash(const IconId &id, size_t seed = 0) { return ::qHash(id.style, seed) ^ ::qHash(id.value); }

#define ICONID(ICON_ENUM) inline IconId iconId(ICON_ENUM em) { return {IconStyle::ICON_ENUM, static_cast<quint64>(em)}; }
    ICONID(AntDesignIcons)
    ICONID(BootstrapIcons)
    ICONID(IconPark)

    inline QLine bottomLine(const QRect &rect, int filletRadius)
    {
        if (int x = rect.center().x(); rect.width() - 2 * filletRadius <= 0)
            return {x, rect.y() + rect.height(), x, rect.y() + rect.height()};
        return {rect.x() + filletRadius, rect.y() + rect.height(), rect.x() + rect.width() - filletRadius, rect.y() + rect.height()};
    }

    inline QImage renderWidgetRegion(QWidget *widget, const QRect &region)
    {
        if (!widget)
            return {};
        // const qreal dpr = widget->devicePixelRatioF();
        QImage image(region.size() /* *dpr */, QImage::Format_ARGB32_Premultiplied);
        // image.setDevicePixelRatio(dpr);
        image.fill(Qt::GlobalColor::black);
        if (const QRect intersected = region.intersected({{0, 0}, widget->size()}); !intersected.isEmpty())
            widget->render(&image, QPoint(0, 0), intersected, QWidget::RenderFlag::DrawWindowBackground);
        return image;
    }

    inline QRect getWidgetRectInAncestor(const QWidget *child, const QWidget *ancestor)
    {
        if (!child || !ancestor)
            return {};
        return {child->mapTo(ancestor, QPoint{0, 0}), child->mapTo(ancestor, QPoint{child->width(), child->height()})};
    }

    inline QImage gaussianBlur(const QImage &src, int radius)
    {
        if (radius <= 0 || src.isNull())
            return src;

        const int width = src.width();
        const int height = src.height();
        const int kernelSize = 2 * radius + 1;
        const qreal sigma = 0.3 * ((kernelSize - 1) * 0.5 - 1) + 0.8;

        // 生成高斯核
        QVector<qreal> kernel(kernelSize);
        qreal sum = 0.0;
        for (int i = 0; i < kernelSize; ++i)
        {
            const qreal x = i - radius;
            const qreal g = qExp(-(x * x) / (2.0 * sigma * sigma));
            kernel[i] = g;
            sum += g;
        }
        for (auto &v: kernel)
            v /= sum;

        QImage temp(src.size(), QImage::Format_ARGB32);
        QImage dst(src.size(), QImage::Format_ARGB32);

        const int bytesPerLine = static_cast<int>(src.bytesPerLine());
        const uchar *srcBits = src.constBits();
        uchar *tempBits = temp.bits();
        uchar *dstBits = dst.bits();

        // -------- 横向模糊 --------
        for (int y = 0; y < height; ++y)
        {
            const uchar *srcLine = srcBits + y * bytesPerLine;
            uchar *tempLine = tempBits + y * bytesPerLine;

            for (int x = 0; x < width; ++x)
            {
                double rSum = 0, gSum = 0, bSum = 0, aSum = 0;

                for (int k = -radius; k <= radius; ++k)
                {
                    const int px = std::clamp(x + k, 0, width - 1);
                    const uchar *p = srcLine + px * 4; // RGBA
                    const double w = kernel[k + radius];

                    bSum += p[0] * w;
                    gSum += p[1] * w;
                    rSum += p[2] * w;
                    aSum += p[3] * w;
                }

                uchar *d = tempLine + x * 4;
                d[0] = static_cast<uchar>(std::lround(bSum));
                d[1] = static_cast<uchar>(std::lround(gSum));
                d[2] = static_cast<uchar>(std::lround(rSum));
                d[3] = static_cast<uchar>(std::lround(aSum));
            }
        }

        // -------- 纵向模糊 --------
        for (int y = 0; y < height; ++y)
        {
            uchar *dstLine = dstBits + y * bytesPerLine;

            for (int x = 0; x < width; ++x)
            {
                double rSum = 0, gSum = 0, bSum = 0, aSum = 0;

                for (int k = -radius; k <= radius; ++k)
                {
                    const int py = std::clamp(y + k, 0, height - 1);
                    const uchar *p = tempBits + py * bytesPerLine + x * 4;
                    const double w = kernel[k + radius];

                    bSum += p[0] * w;
                    gSum += p[1] * w;
                    rSum += p[2] * w;
                    aSum += p[3] * w;
                }

                uchar *d = dstLine + x * 4;
                d[0] = static_cast<uchar>(std::lround(bSum));
                d[1] = static_cast<uchar>(std::lround(gSum));
                d[2] = static_cast<uchar>(std::lround(rSum));
                d[3] = static_cast<uchar>(std::lround(aSum));
            }
        }

        return dst;
    }
}

inline size_t qHash(const QColor &color, size_t seed = 0) { return qHash(color.rgba(), seed); }
#endif //QWIDGET_FANCYUI_ENUM_H
