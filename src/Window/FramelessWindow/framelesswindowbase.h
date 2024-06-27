#ifndef FRAMELESSWINDOWBASE_H
#define FRAMELESSWINDOWBASE_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QVariant>
#include <QScreen>
#include <QWindowStateChangeEvent>
#include "theme.h"
#include "themecolormanagement.h"
#include "precompile_utils/precompile.h"

#include <Windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <wingdi.h>

// This class can only be used on Windows systems
// This class is a window with only a close button
class TitleBarBase;
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
    F_PROTECTED_PROPERTY(float,dpi)
    F_PROTECTED_PROPERTY(Type,type)
    F_PROTECTED_POINTER_PUBLIC_GET(TitleBarBase*,titleBar)//标题栏
    F_PROTECTED_POINTER_PUBLIC_GET(QWidget*,mainArea)//主区域
    F_PROTECTED_POINTER_PUBLIC_GET(QWidget*,clientArea)//客户区
    F_PROTECTED_POINTER_PUBLIC_GET(QVBoxLayout*,verticalLayout)//垂直布局
    F_PROTECTED_POINTER_PUBLIC_GET(QHBoxLayout*,horizontalLayout)//水平布局
    F_PROTECTED_POINTER_PUBLIC_GET(ThemeColorManagement*,themeColorAnimation)//背景色渐变动画管理类
private:
    void initialize(Type type);
};

#endif // FRAMELESSWINDOWBASE_H
