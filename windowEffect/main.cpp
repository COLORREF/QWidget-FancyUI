#include <QApplication>
#include <QCheckBox>
#include <QTimer>
#include <QRandomGenerator>
#include "windoweffect.h"

struct Widget :public QWidget
{
    Widget(QWidget* parent = nullptr):QWidget(parent){setAttribute(Qt::WA_TranslucentBackground);}
};


//仅部分效果支持win10和win11早期版本
//具体未经测试

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //作为子窗口时,可能会不会生效,或产生一些问题,未经测试,请尽量作为主窗口使用

    //修改标题栏颜色(每0.5秒修改成随机颜色)
    QWidget w;
    QTimer timer(&w);
    QApplication::connect(&timer,&QTimer::timeout,&w,[&w]{
        WindowEffect::setTitleBarColor((HWND)w.winId(),
                                       QRandomGenerator::global()->bounded(256),
                                       QRandomGenerator::global()->bounded(256),
                                       QRandomGenerator::global()->bounded(256));
    });
    timer.start(500);
    w.show();

    //云母窗口
    Widget w1;
    WindowEffect::setMicaEffect((HWND)w1.winId());
    w1.show();

    //云母Alt窗口
    Widget w2;
    WindowEffect::setMicaAltEffect((HWND)w2.winId());
    w2.show();

    //亚克力窗口
    Widget w3;
    WindowEffect::setAcrylicEffect((HWND)w3.winId());
    w3.show();

    //win11下调整毛玻璃窗口会卡顿
    //win11下无法使标题栏也生效
    //推荐亚克力窗口

    //毛玻璃窗口(Areo)
    Widget w4;
    WindowEffect::setAreoEffect((HWND)w4.winId());
    w4.show();

    //带颜色的毛玻璃窗口(非Qt事件绘制)
    //透明度值较高时效果和亚克力类似
    Widget w5;
    WindowEffect::setAreoEffect((HWND)w5.winId(),ABGR(150,201,174,255));//使用ABGR宏生成颜色，或直接传入16进制颜色值
    w5.show();

    return a.exec();
}
