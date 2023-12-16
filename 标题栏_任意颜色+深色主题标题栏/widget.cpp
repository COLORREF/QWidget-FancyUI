#include "widget.h"
#include "ui_widget.h"

#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    for(auto& i :findChildren<QSlider*>())
    {
        i->setMinimum(0);
        i->setValue(0);
        i->setMaximum(255);
        connect(i, &QSlider::valueChanged, this, &Widget::changeTitleBarColor);
    }
    connect(ui->DarkMode, &QCheckBox::clicked, this, &Widget::SetTitleBarDarkMode);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeTitleBarColor()
{
    this->SetTitleBarColor(ui->R_Slider->value(),
                           ui->G_Slider->value(),
                           ui->B_Slider->value());
}

bool Widget::SetTitleBarDarkMode(bool enableDarkMode)
{
    BOOL value = enableDarkMode ? TRUE : FALSE; //根据参数确定深色模式的值
    return SUCCEEDED(::DwmSetWindowAttribute(
        (HWND)this->winId(),
        DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
        &value,
        sizeof(BOOL)));// 返回设置或取消深色模式是否成功
}

bool Widget::SetTitleBarColor(int red, int green, int blue)
{
    COLORREF color = RGB(red, green, blue);// 使用RGB宏创建COLORREF
    return SUCCEEDED(::DwmSetWindowAttribute(
        (HWND)this->winId(),
        DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
        &color,
        sizeof(COLORREF)));
}
