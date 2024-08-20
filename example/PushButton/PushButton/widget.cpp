#include "widget.h"
#include "ui_widget.h"
#include <QColorDialog>
#include "utils/customcontrol.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->pushButton_9,&QPushButton::clicked,this,[](){
        QColor color = QColorDialog::getColor(ControlColors::controlColors()->prominence(), nullptr, "选择主题色");
        if (color.isValid())
            ControlColors::controlColors()->setProminence(color);
    });

}

Widget::~Widget()
{
    delete ui;
}
