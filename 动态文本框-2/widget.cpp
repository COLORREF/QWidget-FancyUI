#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //函数功能请看源码注释
    ui->frame->setTipsText("请输入文字");
    ui->frame->setTipsFont(QFont("微软雅黑",12),Qt::cyan);
    ui->frame->setTipsFontColor(Qt::cyan);
    ui->frame->setEditExpansionHeight(35);                      //！！！！请设置输入框展开高度小于QFrame高度
    ui->frame->setEditBackgroundColor(QColor(152,169,238));
    ui->frame->setBorderColor(QColor(210,67,115),2);
    ui->frame->setBorderRadius(10);
    ui->frame->setEditFont(QFont("微软雅黑",13));
    ui->frame->setEditFontColor(QColor(255,255,255));
}

Widget::~Widget()
{
    delete ui;
}

