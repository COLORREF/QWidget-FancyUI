#include <QApplication>
#include <QWidget>
#include <QColorDialog>
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

// 设置或取消窗口边框(仅默认窗口(浅色主题)有效,设置过标题栏颜色后无效,标题栏仍然保留)// 没啥用(实际上就是改变边框颜色)
BOOL SetWindowBorders(const HWND& hWnd, bool enableBorders)
{
    COLORREF color = enableBorders ? DWMWA_COLOR_DEFAULT : DWMWA_COLOR_NONE;
    return SUCCEEDED(::DwmSetWindowAttribute(
        hWnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, &color, sizeof(COLORREF)));
}

//设置标题文字颜色
BOOL SetsHeaderTextColor(const HWND& hWnd, COLORREF color)//如果要恢复系统默认的颜色请将color的值设为：0xFFFFFFFF
{
    return SUCCEEDED(::DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_TEXT_COLOR,&color,sizeof(COLORREF)));
}

//设置边框颜色
BOOL SetBorderColor(const HWND& hWnd, COLORREF color)
{
    return SUCCEEDED(::DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR,&color,sizeof(COLORREF)));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    QColorDialog colorDialog(&w);
    colorDialog.show();
    QApplication::connect(&colorDialog,&QColorDialog::QColorDialog::currentColorChanged,&w,[&](QColor color){
        SetBorderColor((HWND)w.winId(),RGB(color.red(),color.green(),color.blue()));
    });
    QApplication::connect(&colorDialog,&QColorDialog::QColorDialog::currentColorChanged,&w,[&](QColor color){
        SetsHeaderTextColor((HWND)w.winId(),RGB(color.red(),color.green(),color.blue()));
    });
    w.show();
    return a.exec();
}
