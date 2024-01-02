#include <QApplication>
#include <QWidget>
#include "ui_widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet("QWidget{background-color: rgb(26, 32, 50);}");

    QWidget w;

    Ui::Widget ui;
    ui.setupUi(&w);

    ui.lineEdit_2->setFontColor(Qt::GlobalColor::white);
    ui.lineEdit_2->setPlaceholderTextColor(QColor(190, 192, 194));
    ui.lineEdit_2->setFont(QFont("微软雅黑"));
    ui.lineEdit_2->setPlaceholderText("请输入文字");
    ui.lineEdit_2->setLineColor(QColor(66, 66, 66), QColor(255, 0, 0));

    ui.lineEdit_3->setFontColor(Qt::GlobalColor::white);
    ui.lineEdit_3->setPlaceholderTextColor(QColor(190, 192, 194));
    ui.lineEdit_3->setFont(QFont("微软雅黑"));
    ui.lineEdit_3->setPlaceholderText("请输入文字");
    ui.lineEdit_3->setLineColor(QColor(66, 66, 66), QColor(255, 150, 0));

    w.show();
    return a.exec();
}
