#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}

// "-platformwindows:darkmode=0"
// #include <QStyleHints>
// auto colorScheme = QApplication::styleHints()->colorScheme();
// qDebug()<<(colorScheme == Qt::ColorScheme::Dark);
// QApplication::setStyle(QApplication::style()->objectName());
// qApp->setPalette(QApplication::style()->standardPalette());
