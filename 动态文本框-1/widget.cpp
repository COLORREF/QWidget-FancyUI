#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->lineEdit_2->setFontColor(Qt::white);
    ui->lineEdit_2->setFont(QFont("微软雅黑"));

    ui->lineEdit_3->setFontColor(Qt::white);
    ui->lineEdit_3->setFont(QFont("微软雅黑"));

    ui->lineEdit_2->setLineColor(QColor(66, 66, 66), QColor(255, 0, 0));
    ui->lineEdit_3->setLineColor(QColor(66, 66, 66), QColor(255, 0, 0));
}

Widget::~Widget()
{
    delete ui;
}

