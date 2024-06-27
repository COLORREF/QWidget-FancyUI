#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.setStyleSheet("QWidget{background-color: rgb(107, 107, 107);}");

    w.show();
    return a.exec();
}
