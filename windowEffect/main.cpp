#include <QApplication>

//for example
#include <QCheckBox>
#include <QTimer>
#include <QRandomGenerator>

#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

#include "compositionwindoweffect.h"
#include "dwmwindoweffect.h"

//MinGW编译请查看 windowmanager.cpp中的注释

struct Widget :public QWidget
{
    Widget(QWidget* parent = nullptr):QWidget(parent)
    {
        setAttribute(Qt::WA_TranslucentBackground);
        // setWindowFlags(Qt::FramelessWindowHint);
    }
};

//仅部分效果支持win10和win11早期版本
//作为子窗口时,可能不会生效,或产生一些问题,未经测试,请尽量作为主窗口使用
//具体未经测试

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    //修改标题栏和边框颜色(每0.5秒修改成随机颜色)
    QWidget w;
    QTimer timer(&w);
    w.connect(&timer,&QTimer::timeout,&w,[&w]{
        DwmWindowEffect::setTitleBarColor(
            (HWND)w.winId(),
            RGB(QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256))
            );
        DwmWindowEffect::setBorderColor(
            (HWND)w.winId(),
            RGB(QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256),
                QRandomGenerator::global()->bounded(256))
            );
    });
    timer.start(500);
    w.show();

    //云母窗口
    Widget w1;
    DwmWindowEffect::setMicaEffect((HWND)w1.winId());
    w1.setWindowTitle("云母");
    QCheckBox box_1("深色模式",&w1);
    w1.connect(&box_1,&QCheckBox::toggled,&w1,[&w1](bool checked){
        if(checked)DwmWindowEffect::setWindowDarkMode((HWND)w1.winId());
        else DwmWindowEffect::setWindowLightMode((HWND)w1.winId());
    });
    box_1.move(w1.width() / 2 - box_1.width() / 2, w1.height() / 2 - box_1.height() / 2);
    w1.show();

    //云母Alt窗口
    Widget w2;
    DwmWindowEffect::setMicaAltEffect((HWND)w2.winId());
    w2.setWindowTitle("云母Alt");
    QCheckBox box_2("深色模式",&w2);
    w2.connect(&box_2,&QCheckBox::toggled,&w2,[&w2](bool checked){
        if(checked)DwmWindowEffect::setWindowDarkMode((HWND)w2.winId());
        else DwmWindowEffect::setWindowLightMode((HWND)w2.winId());
    });
    box_2.move(w2.width() / 2 - box_2.width() / 2, w2.height() / 2 - box_2.height() / 2);
    w2.show();

    //无边框 亚克力窗口
    Widget w3;
    DwmWindowEffect::setAcrylicEffect((HWND)w3.winId());
    WindowManager::disableTitleBar((HWND)w3.winId());//无边框有缺陷，属于win32问题，暂时未找到好的解决方案，详细请查看注释
    w3.setWindowTitle("亚克力");
    QCheckBox box_3("深色模式",&w3);
    w3.connect(&box_3,&QCheckBox::toggled,&w3,[&w3](bool checked){
        if(checked)DwmWindowEffect::setWindowDarkMode((HWND)w3.winId());
        else DwmWindowEffect::setWindowLightMode((HWND)w3.winId());
    });
    box_3.move(w3.width() / 2 - box_3.width() / 2, w3.height() / 2 - box_3.height() / 2);
    w3.show();

    //win11下调整毛玻璃窗口大小会卡顿
    //win11下无法使标题栏也生效
    //推荐DwmWindowEffect::setAcrylicEffect设置亚克力效果

    //毛玻璃窗口(Areo)
    Widget w4;
    CompositionWindowEffect::setAreoEffect((HWND)w4.winId());
    w4.setWindowTitle("毛玻璃");
    w4.show();

    //带颜色的毛玻璃窗口(非Qt事件绘制)
    //透明度值较高时效果和亚克力类似
    Widget w5;
    CompositionWindowEffect::setAreoEffect((HWND)w5.winId(),ABGR(150,201,174,255));//使用ABGR宏生成颜色，或直接传入16进制颜色值
    w5.setWindowTitle("带颜色毛玻璃");
    w5.show();

    return a.exec();
}
