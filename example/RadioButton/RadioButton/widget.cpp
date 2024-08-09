#include "widget.h"
#include "ui_widget.h"
#include <QColorDialog>
#include "utils/controlcolors.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    auto checkboxs = findChildren<QCheckBox*>();
    auto radiobtns = findChildren<RadioButton*>();
    for(int i = 0; i < 4; i++)
        connect(checkboxs[i],&QCheckBox::clicked,this,[r = radiobtns[i]](bool checked){r->setEnabled(!checked);});

    connect(ui->pushButton,&QPushButton::clicked,this,[](){
        QColor color = QColorDialog::getColor(ControlColors::controlColors()->prominence(), nullptr, "选择主题色颜色");//初始默认颜
        if (color.isValid())
            ControlColors::controlColors()->setProminence(color);
    });
}

Widget::~Widget()
{
    delete ui;
}
