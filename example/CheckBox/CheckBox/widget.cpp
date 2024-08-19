#include "widget.h"
#include "ui_widget.h"
#include "utils/customcontrol.h"
#include <QColorDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    auto checkboxs = findChildren<QCheckBox*>();
    auto radiobtns = findChildren<QRadioButton*>();
    for(int i = 0; i < 4; i++)
        connect(radiobtns[i],&QRadioButton::toggled,this,[c = checkboxs[i]](bool checked){c->setEnabled(!checked);});

    connect(ui->pushButton,&QPushButton::clicked,this,[](){
        QColor color = QColorDialog::getColor(ControlColors::controlColors()->prominence(), nullptr, "选择主题色");
        if (color.isValid())
            ControlColors::controlColors()->setProminence(color);
    });

}

Widget::~Widget()
{
    delete ui;
}
