#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowIcon(QIcon(":/logo.ico"));   //设置程序图标
    w.setWindowTitle("计算器");              //设置窗口标题
    w.show();                               //显示窗口
    return a.exec();                        //阻塞循环，等待程序退出
}
