#include <QApplication>
#include <QWidget>
#include "DynamicUnderlineLineEdit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.setStyleSheet("QWidget{background-color: rgb(26, 32, 50);}");

    DynamicUnderlineLineEdit *lineEdit_1 = new DynamicUnderlineLineEdit(&w);
    lineEdit_1->setGeometry(QRect(160, 134, 201, 31));
    lineEdit_1->setFontColor(Qt::GlobalColor::white);
    lineEdit_1->setPlaceholderTextColor(QColor(190, 192, 194));
    lineEdit_1->setFont(QFont("微软雅黑"));
    lineEdit_1->setPlaceholderText("请输入文字");
    lineEdit_1->setLineColor(QColor(66, 66, 66), Qt::GlobalColor::red);


    DynamicUnderlineLineEdit *lineEdit_2 = new DynamicUnderlineLineEdit(&w);
    lineEdit_2->setGeometry(QRect(160, 190, 201, 31));
    lineEdit_2->setFontColor(Qt::GlobalColor::cyan);
    lineEdit_2->setPlaceholderTextColor(QColor(229,199,158));
    lineEdit_2->setFont(QFont("微软雅黑"));
    lineEdit_2->setPlaceholderText("请输入文字");
    lineEdit_2->setLineColor(QColor(66, 66, 66), QColor(255, 150, 0));

    w.show();
    return a.exec();
}
