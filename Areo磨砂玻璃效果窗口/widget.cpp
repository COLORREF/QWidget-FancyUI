#include "widget.h"
#include "ui_widget.h"
#include "SetWindowCompositionAttribute.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    for(auto& i :findChildren<QSlider*>())
    {
        i->setMinimum(0);
        i->setValue(0);
        i->setMaximum(255);
        connect(i,&QSlider::valueChanged,this,&Widget::changeColor);
    }

    this->setAttribute(Qt::WA_TranslucentBackground);// 设置窗口背景透明
    // this->setStyleSheet("background:transparent");//使用样式表使窗口透明也可以

    //win11和win10效果不一样
    //win11窗口标题栏无变化(区别与win10)
    this->Areo();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::Areo()
{
    if (SetWindowCompositionAttribute)//如果不为空指针
    {
        ACCENT_POLICY accent1 = { ACCENT_STATE::ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };//Areo效果
        WINDOWCOMPOSITIONATTRIBDATA data1{ WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY, &accent1,sizeof(accent1) };
        SetWindowCompositionAttribute((HWND)this->winId(), &data1);
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setBrush(QColor(ui->Slider_R->value(),
                            ui->Slider_G->value(),
                            ui->Slider_B->value(),
                            ui->Slider_A->value()));
    painter.setPen(Qt::PenStyle::NoPen);
    painter.drawRect(this->rect());
}

void Widget::changeColor()
{
    this->update();
    ui->label_5->setText(QString::number(ui->Slider_A->value()));
}
