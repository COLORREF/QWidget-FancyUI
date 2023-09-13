#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    ,timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->pushbutton->setmove_speed(600);//设置按钮从左端移动到右端的时间1200毫秒

    //请确保按钮宽高比为18:7(约为2.57)左右
    ui->pushbutton->resize(360,140);// !!!
    //！！！！！


    timer->setInterval(ui->pushbutton->getmove_speed()/240/2);//“240”是背景色最终的透明度，“2”是每次触发定时器背景色增加的值
    //这个运算的结果意思是：让背景色的渐变切换速度和按钮太阳或月亮的移动保持一致
    //可以自行微调

    connect(ui->pushbutton->btn,&day_to_night_btn::clicked,this,[=]{timer->start();});

    connect(timer,&QTimer::timeout,this,[=](){
        if(ui->pushbutton->Is_day() && is_day)
        {
            if(night_A<240)
                night_A+=2;
            update();
        }
        else if(!ui->pushbutton->Is_day() && !is_day)
        {
            if(night_A>0)
                night_A-=2;
            update();
        }

        if(night_A == 240)
        {
            is_day = false;
            timer->stop();
        }
        else if(night_A == 0)
        {
            is_day = true;
            timer->stop();
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addRoundedRect(rect(),0,0);
    painter.setClipPath(path);//设置绘制路径

    //白天
    painter.setBrush(QColor(240,248,255));
    painter.drawRoundedRect(rect(),0,0);

    //黑夜
    painter.setBrush(QColor(50,50,50,night_A));
    painter.drawRoundedRect(rect(),0,0);



//    int x = 0;
//    int y = 0;
//    double radius =400;
//    radius = ui->pushbutton->btn->sun_moon_radius;
//    double t = 400/(ui->pushbutton->btn->sun_moon_radius);

//    painter.setBrush(QColor(255,195,35));
//    painter.drawEllipse(x,y,radius,radius);

//    //高光
//    QRadialGradient radialGradient1(radius/2, radius/2, 250/t,   175/t,160/t, 200/t);//中心,渐变半径, 焦点,焦半径
//    radialGradient1.setColorAt(0, QColor(255,195,35));
//    radialGradient1.setColorAt(1, QColor(0,0,0));

//    painter.setBrush(QBrush(radialGradient1));
//    painter.drawEllipse(x,y,radius,radius);

    //阴影
//    QRadialGradient radialGradient2(ui->horizontalSlider_9->value(), ui->horizontalSlider_15->value(), ui->horizontalSlider_8->value(),   ui->horizontalSlider_6->value(),ui->horizontalSlider_7->value(), ui->horizontalSlider_5->value());//中心,渐变半径, 焦点,焦半径
//    radialGradient2.setColorAt(0, QColor(255,195,35,100));
//    radialGradient2.setColorAt(1, QColor(255,230,80,200));

//    painter.setBrush(QBrush(radialGradient2));
//    painter.drawEllipse(0,0,400,400);

//    painter.setPen(Qt::red);
//    painter.drawLine(200,200,200,400);
//    painter.drawLine(0,200,200,200);

}
