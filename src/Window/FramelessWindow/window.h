#ifndef WINDOW_H
#define WINDOW_H

// This file can only be used on Windows systems

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QVariant>
#include <QScreen>
#include <QWindowStateChangeEvent>
#include <QDebug>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QStackedWidget>

// #include <windows.devices.lights.effects.h>
#include <Windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <wingdi.h>

#include "utils/theme.h"
#include "utils/precompile.h"
#include "utils/animation.h"
#include "utils/dwmwindoweffect.h"
#include "utils/compositionwindoweffect.h"

#include "titlebar.h"
#define ABGR(a, b, g, r) ((COLORREF)(((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)))

class FramelessWindowBase : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWindowBase(QWidget *parent = nullptr);
    void setThemeColor(const QColor &light = QColor(240, 243, 249), const QColor &dark = QColor(0, 10, 37));
    QColor lightThemeColor();
    QColor darkThemeColor();
    void setThemeColorToggleTime(int msecs = 300); // 设置主题色切换时间
    virtual void deleteTitleBar();                 // 做了安全检查，调用此函数后，标题栏指针无效，通过标题栏指针获取到的按钮指针也无效，使用这些无效指针会使程序崩溃
    void showSystemTitleBarMenu();                 // 在调用时的鼠标位置弹出系统默认的标题栏菜单，选项无实际作用，需要在 nativeEvent中手动处理消息
protected slots:
    virtual void onThemeChange(Theme::Type type);

protected:
    enum class Type
    {
        Base,
        Simple,
        Standard
    };

    explicit FramelessWindowBase(QWidget *parent, Type type);

    F_NATIVEEVENT;
    F_PAINTEVENT;
    F_CHANGEEVENT;

    int X_BORDER_WIDTH;
    int Y_BORDER_WIDTH;
    F_PROTECTED_PROPERTY(float, dpi)
    F_PROTECTED_PROPERTY(Type, type)
    F_PROTECTED_POINTER_PUBLIC_GET(TitleBarBase *, titleBar)                    // 标题栏
    F_PROTECTED_POINTER_PUBLIC_GET(QWidget *, mainArea)                         // 主区域
    F_PROTECTED_POINTER_PUBLIC_GET(QWidget *, clientArea)                       // 客户区
    F_PROTECTED_POINTER_PUBLIC_GET(QVBoxLayout *, verticalLayout)               // 垂直布局
    F_PROTECTED_POINTER_PUBLIC_GET(QHBoxLayout *, horizontalLayout)             // 水平布局
    F_PROTECTED_POINTER_PUBLIC_GET(ThemeColorManagement *, themeColorAnimation) // 背景色渐变动画管理类
private:
    void initialize(Type type);
};

class SimpleFramelessWindow : public FramelessWindowBase
{
    Q_OBJECT
public:
    explicit SimpleFramelessWindow(QWidget *parent = nullptr);
    virtual void deleteTitleBar() override;

protected:
    explicit SimpleFramelessWindow(QWidget *parent, Type type);

    F_NATIVEEVENT;
    F_CHANGEEVENT;
    MaximizeButton *maximize_button;
};

class FramelessWindow : public SimpleFramelessWindow
{
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *parent = nullptr);
public slots:
    void setWindowIcon(const QPixmap &icon);
    void setWindowTitle(const QString &title);
    void setTitleFont(const QFont &font);
    void setTitleColor(const QColor &globalColor);
    void setTitleColor(const QColor &lightColor, const QColor &darkColor);
};

enum class WindowEffectType
{
    ACRYLIC,          // 亚克力
    AREO,             // 毛玻璃
    FULLYTRANSPARENT, // 全透明
    MICA,             // 云母
    MICAALT,          // 云母ALT
    SYSTEMCOLOR,      // 系统色
    DEFAULT,          // 默认
};

class TransparentEffectWindowBase : public FramelessWindow
{
    Q_OBJECT
protected:
    explicit TransparentEffectWindowBase(QWidget *parent, WindowEffectType effectType);

    F_PAINTEVENT;

    // 云母
    inline void setMicaEffect(const HWND hWnd)
    {
        this->setTransparency();
        DwmWindowEffect::setMicaEffect(hWnd);
        MARGINS margins = {INT_MAX, 0, 0, 0};
        ::DwmExtendFrameIntoClientArea(hWnd, &margins);
    }

    // 云母Alt
    inline void setMicaAltEffect(const HWND hWnd)
    {
        this->setTransparency();
        DwmWindowEffect::setMicaAltEffect(hWnd);
        MARGINS margins = {INT_MAX, 0, 0, 0};
        ::DwmExtendFrameIntoClientArea(hWnd, &margins);
    }

    // 亚克力
    inline void setAcrylicEffect(const HWND hWnd)
    {
        this->setTransparency();
        DwmWindowEffect::setAcrylicEffect(hWnd);
        MARGINS margins = {INT_MAX, 0, 0, 0};
        ::DwmExtendFrameIntoClientArea(hWnd, &margins);
    }

    // 跟随系统颜色
    inline void setFollowingSystemColor(const HWND hwnd)
    {
        this->setTransparency();
        MARGINS margins = {0, 0, 1, 0};
        ::DwmExtendFrameIntoClientArea(hwnd, &margins);
        CompositionWindowEffect::setFollowingSystemColor((HWND)(hwnd));
    }

    // Areo
    inline void setAreoEffect(const HWND hwnd)
    {
        this->setTransparency();
        MARGINS margins = {0, 0, 1, 0};
        ::DwmExtendFrameIntoClientArea(hwnd, &margins);
        CompositionWindowEffect::setAreoEffect((HWND)(hwnd));
    }

    // Areo,16进制颜色
    inline void setAreoEffect(const HWND hwnd, COLORREF abgr)
    {
        this->setTransparency();
        MARGINS margins = {0, 0, 1, 0};
        ::DwmExtendFrameIntoClientArea(hwnd, &margins);
        CompositionWindowEffect::setAreoEffect(hwnd, abgr);
    }

    // 客户区全透明
    inline void setClientAreaFullyTransparent(const HWND hwnd)
    {
        this->setTransparency();
        MARGINS margins = {0, 0, 1, 0};
        ::DwmExtendFrameIntoClientArea(hwnd, &margins);
        CompositionWindowEffect::setClientAreaFullyTransparent(hwnd);
    }

    WindowEffectType effect_type;

private:
    inline void setTransparency()
    {
        ((MinimizeButton *)(((StandardTitleBar *)(this->_titleBar))->minimizeButton()))->setIsClearBeforeNewPaint(true);
        ((StandardTitleBar *)(this->_titleBar))->closeButton()->setIsClearBeforeNewPaint(true);
        ((MaximizeButton *)(((StandardTitleBar *)(this->_titleBar))->maximizeButton()))->setIsClearBeforeNewPaint(true);
        (((StandardTitleBar *)(this->_titleBar))->titleLabel())->setIsClearBeforeNewPaint(true);
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
        this->setWindowFlags((this->windowFlags()) & (~Qt::WindowType::FramelessWindowHint));
#endif
        this->setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
        if (this->_themeColorAnimation)
        {
            delete this->_themeColorAnimation;
            _themeColorAnimation = nullptr;
        }
    }
};

class AcrylicEffectWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit AcrylicEffectWindow(QWidget *paernt = nullptr)
        : TransparentEffectWindowBase{paernt, WindowEffectType::ACRYLIC}
    {
    }
};

class FullyTransparentWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit FullyTransparentWindow(QWidget *parent = nullptr)
        : TransparentEffectWindowBase{parent, WindowEffectType::FULLYTRANSPARENT}
    {
    }
};

class MicaEffectWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit MicaEffectWindow(QWidget *parent = nullptr)
        : TransparentEffectWindowBase{parent, WindowEffectType::MICA}
    {
    }
};

class MicaAltEffectWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit MicaAltEffectWindow(QWidget *parent = nullptr)
        : TransparentEffectWindowBase{parent, WindowEffectType::MICAALT}
    {
    }
};

class SystemColorWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit SystemColorWindow(QWidget *parent = nullptr)
        : TransparentEffectWindowBase{parent, WindowEffectType::SYSTEMCOLOR}
    {
    }
};

class AreoWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit AreoWindow(QWidget *parent = nullptr);
    explicit AreoWindow(COLORREF abgr, QWidget *parent = nullptr);
public slots:
    void changeColor(COLORREF abgr = 0);

private:
    long long window_color = -1;
    void init(long long abgr = -1);
};

class LightAndDarkWidget : public QWidget
{
    Q_OBJECT
public:
    LightAndDarkWidget(QWidget *parent, QColor light = QColor(248, 249, 252, 127), QColor dark = QColor(255, 255, 255, 8));

    F_PAINTEVENT;
    F_PROTECTED_POINTER_PUBLIC_GET(ThemeColorManagement *, themeColorManagement)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(bool, isClearBeforeNewPaint, IsClearBeforeNewPaint, ) // 是否在新的绘制前清空旧的区域
};

class NTQQWindow : public SimpleFramelessWindow
{
    Q_OBJECT
public:
    explicit NTQQWindow(QWidget *parent = nullptr);
public slots:
    void setUnmaskSidebarColor(bool isUnmask = false); // 取消侧边栏的遮罩颜色

    F_PRIVATE_POINTER_PUBLIC_GET(LightAndDarkWidget *, sidebar) // 侧边栏
    F_PAINTEVENT;
};

class MSFilletedCornerWidget : public LightAndDarkWidget
{
public:
    using LightAndDarkWidget::LightAndDarkWidget;
    F_PAINTEVENT;
};

class MSWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit MSWindow(QWidget *parent = nullptr, WindowEffectType effectType = WindowEffectType::MICA);

    F_PRIVATE_PROPERTY(QHBoxLayout *, horizontalLayout)
    F_PROTECTED_POINTER_PUBLIC_GET(MSFilletedCornerWidget *, clientArea) // 客户区
    F_PROTECTED_POINTER_PUBLIC_GET(QWidget *, sidebar)                   // 侧边栏
};

// Code in writing
class AnimationStackedWidget : public QStackedWidget
{
public:
    using QStackedWidget::QStackedWidget;
};
//

class AdaptiveLayoutWindow : public MSWindow
{
    Q_OBJECT
public:
    explicit AdaptiveLayoutWindow(QWidget *parent = nullptr, WindowEffectType effectType = WindowEffectType::MICA);

    F_RESIZEEVENT;
    F_PRIVATE_PROPERTY(QVariantAnimation *, animation)
    F_PRIVATE_PROPERTY(bool, isExpand)

private slots:
    void checkWidth();
};

#endif // WINDOW_H
