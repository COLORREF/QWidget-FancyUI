#include <QApplication>
#include <QWidget>
#include "borderlesswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BorderlessWindow w;
    w.show();
    return a.exec();
}
