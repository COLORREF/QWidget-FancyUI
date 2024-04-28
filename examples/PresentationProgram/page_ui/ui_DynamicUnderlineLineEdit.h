#ifndef UI_DYNAMICUNDERLINELINEEDIT_H
#define UI_DYNAMICUNDERLINELINEEDIT_H
#include <QWidget>
#include <QRect>
#include <QColor>
#include <QFont>
#include <QPen>
#include "../../DynamicUnderlineLineEdit/DynamicUnderlineLineEdit.h"

class Ui_DynamicUnderlineLineEdit
{
public:
    DynamicUnderlineLineEdit *lineEdit_1;
    DynamicUnderlineLineEdit *lineEdit_2;

    void setupUi(QWidget *parent)
    {
        lineEdit_1 = new DynamicUnderlineLineEdit(parent);
        lineEdit_2 = new DynamicUnderlineLineEdit(parent);

        lineEdit_1->setGeometry(QRect(160, 134, 201, 31));
        lineEdit_1->setTextColor(Qt::white);
        lineEdit_1->setPlaceholderTextColor(QColor(190, 192, 194));
        lineEdit_1->setFont(QFont("微软雅黑"));
        lineEdit_1->setText("请输入文字_可以选中");
        lineEdit_1->setLinePen(QPen(Qt::red, 2)); // 第二个参数默认值宽度为2,请确保两个画笔的宽度一样

        lineEdit_2->setGeometry(QRect(160, 190, 201, 31));
        lineEdit_2->setTextColor(Qt::cyan);
        lineEdit_2->setPlaceholderTextColor(QColor(229, 199, 158));
        lineEdit_2->setFont(QFont("微软雅黑"));
        lineEdit_2->setPlaceholderText("请输入文字_无法选中");
        lineEdit_2->setLinePen(QPen(QColor(255, 150, 0), 2), QPen(Qt::lightGray, 2)); // 请确保两个画笔的宽度一样

        // parent->setObjectName("page_DynamicUnderlineLineEdit");
        parent->setStyleSheet("QWidget{background-color: rgb(26, 32, 50);}");
    }
};
#endif // UI_DYNAMICUNDERLINELINEEDIT_H
