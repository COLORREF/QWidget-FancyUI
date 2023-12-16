#include "widget.h"
#include "ui_widget.h"

#include "SetWindowCompositionAttribute.h"
#include <dwmapi.h>
#pragma comment ( lib,"user32.lib" )
#pragma comment ( lib,"dwmapi.lib" )

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    this->ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    // this->setStyleSheet("background:transparent");

    this->setMicaEffect();//普通云母效果
    this->setWindowTitle("Mica 云母效果窗口");

    // this->setMicaEffect(true);//Alt云母效果
    // this->setWindowTitle("Mica Alt 云母Alt效果窗口");

    this->connect(ui->DarkTheme,&QCheckBox::clicked,this,&Widget::SetDarkMode);
    this->connect(ui->DarkTheme,&QCheckBox::clicked,this,[=](bool isClicked){
        if(isClicked)
            ui->DarkTheme->setStyleSheet("color:white;");
        else
            ui->DarkTheme->setStyleSheet("");
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setMicaEffect(bool isAlt)
{
    MARGINS margins{ -1, -1, -1, -1 };//四个边框的保持宽度
    ::DwmExtendFrameIntoClientArea(HWND(this->winId()), &margins);

    WINDOWCOMPOSITIONATTRIBDATA winCompAttrData;
    ACCENT_POLICY accentPolicy;

    winCompAttrData.Attribute = WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY;
    accentPolicy.AccentState = ACCENT_STATE::ACCENT_ENABLE_HOSTBACKDROP;
    SetWindowCompositionAttribute(HWND(this->winId()), &winCompAttrData);

    /*来自微软官方文档--https://learn.microsoft.com/zh-cn/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute :
        DWMWA_SYSTEMBACKDROP_TYPE与 DwmGetWindowAttribute 或 DwmSetWindowAttribute 一起使用。
        检索或指定窗口的系统绘制背景材料，包括非工作区后面。
        参数三指向 DWM_SYSTEMBACKDROP_TYPE 类型的值。
        ==================!!!!! 从 Windows 11 版本 22621 开始支持此值。!!!!!===============
    */

    // 低于此版本，请自行测试，我（代码编写者）没有低于此版本的 Win11系统环境，没有进行测试

    //enum DWM_SYSTEMBACKDROP_TYPE
    //{
    //	DWMSBT_AUTO,             // [Default] Let DWM automatically decide the system-drawn backdrop for this window.
    //	DWMSBT_NONE,             // Do not draw any system backdrop.
    //	DWMSBT_MAINWINDOW,       // Draw the backdrop material effect corresponding to a long-lived window.
    //	DWMSBT_TRANSIENTWINDOW,  // Draw the backdrop material effect corresponding to a transient window.
    //	DWMSBT_TABBEDWINDOW,     // Draw the backdrop material effect corresponding to a window with a tabbed title bar.
    //};

    DWM_SYSTEMBACKDROP_TYPE system_backdrop_type = isAlt ? DWM_SYSTEMBACKDROP_TYPE::DWMSBT_TABBEDWINDOW : DWM_SYSTEMBACKDROP_TYPE::DWMSBT_MAINWINDOW;
    ::DwmSetWindowAttribute((HWND)(this->winId()), DWMWINDOWATTRIBUTE::DWMWA_SYSTEMBACKDROP_TYPE, &system_backdrop_type, sizeof(DWM_SYSTEMBACKDROP_TYPE));
}

bool Widget::SetDarkMode(bool enableDarkMode)
{
    BOOL value = enableDarkMode ? TRUE : FALSE; //根据参数确定深色模式的值
    return SUCCEEDED(::DwmSetWindowAttribute(
        (HWND)this->winId(),
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL)));// 返回设置或取消深色模式是否成功
}
