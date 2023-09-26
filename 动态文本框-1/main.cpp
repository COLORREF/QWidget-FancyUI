#include <QApplication>
#include <QPixmap>
#include <QPalette>

#include "widget.h"
#include "DynamicLineEdit.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
