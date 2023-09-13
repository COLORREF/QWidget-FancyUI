#include "day_to_night_btn.h"
#include <QDebug>

day_to_night_btn::day_to_night_btn(QWidget *parent)
    :QPushButton(parent)
    ,timer(new QTimer(this))
{
    resize(width(),height());//调用resize，在resize事件中初始化，减少代码量

    connect(this,&day_to_night_btn::clicked,this,[=](){timer->start();});

    //坐标的偏移，达到移动效果
    connect(timer,&QTimer::timeout,this,[=](){
        if(is_day)
        {
            sun_moon_x += move_value;
            meteor_crater1_x += move_value;
            meteor_crater2_x += move_value;
            meteor_crater3_x += move_value;

            if(A<255)
                A+=5;
        }
        else
        {
            sun_moon_x -= move_value;
            meteor_crater1_x -= move_value;
            meteor_crater2_x -= move_value;
            meteor_crater3_x -= move_value;
            if(A>0)
                A-=5;
        }
        if(sun_moon_x>(width()-radius) && is_day)
        {
            is_day = false;
            timer->stop();
        }
        else if(sun_moon_x<radius && !is_day)
        {
            is_day = true;
            timer->stop();
        }
        update();
    });

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(5, 5);                        //阴影的偏移量（右5，下5）
    shadow->setColor(QColor(0, 0, 0, 100));         //阴影的颜色
    shadow->setBlurRadius(10);                      //控制阴影的模糊程度（光源距离，可以这么理解？）
    setGraphicsEffect(shadow);
}

void day_to_night_btn::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    radius = height()/2;
    sun_moon_radius = 77*radius/95;
    sun_moon_x = radius;
    sun_moon_y = radius;
    sun_moon_S = (width()-radius) - sun_moon_x;

    meteor_crater1_x = 0.68*sun_moon_x;
    meteor_crater1_y = 1.14*sun_moon_x;
    meteor_crater1_radius = 0.36*sun_moon_radius;

    meteor_crater2_x = 1.16*sun_moon_x;
    meteor_crater2_y = 0.68*sun_moon_y;
    meteor_crater2_radius = 0.23*sun_moon_radius;

    meteor_crater3_x = 1.34*sun_moon_x;
    meteor_crater3_y = 1.3*sun_moon_y;
    meteor_crater3_radius = meteor_crater2_radius;

    timer->setInterval(2*move_speed/sun_moon_S);
}

void day_to_night_btn::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addRoundedRect(rect(),radius,radius);
    painter.setClipPath(path);//设置绘制路径

    //太阳
    painter.setBrush(QColor(255,195,35));
    painter.drawEllipse(QPoint(sun_moon_x,sun_moon_y),sun_moon_radius,sun_moon_radius);

    //月亮
    painter.setBrush(QColor(195,200,210,A));
    painter.drawEllipse(QPoint(sun_moon_x,sun_moon_y),sun_moon_radius,sun_moon_radius);

    //可以改成成员变量以提高性能？（大概？） 但每次设置前记得清空或者其他一些操作，对于绘制路径也是同样的
    //作为临时变量让系统管理更加方便，但如果是大量临时对象的创建和析构可能会降低性能
    QRadialGradient radialGradient1(meteor_crater1_x, meteor_crater1_y, meteor_crater1_radius);
    radialGradient1.setColorAt(0, QColor(150,160,180,A));
    radialGradient1.setColorAt(0.8, QColor(150,160,180,A));
    radialGradient1.setColorAt(1, QColor(108,115,130,A));

    QRadialGradient radialGradient2(meteor_crater2_x, meteor_crater2_y, meteor_crater2_radius);
    radialGradient2.setColorAt(0, QColor(150,160,180,A));
    radialGradient2.setColorAt(0.7, QColor(150,160,180,A));
    radialGradient2.setColorAt(1, QColor(108,115,130,A));


    QRadialGradient radialGradient3(meteor_crater3_x, meteor_crater3_y, meteor_crater3_radius);
    radialGradient3.setColorAt(0, QColor(150,160,180,A));
    radialGradient3.setColorAt(0.7, QColor(150,160,180,A));
    radialGradient3.setColorAt(1, QColor(108,115,130,A));

    //陨石坑1
    painter.setBrush(radialGradient1);
    painter.drawEllipse(QPoint(meteor_crater1_x,meteor_crater1_y),meteor_crater1_radius,meteor_crater1_radius);

    //陨石坑2
    painter.setBrush(radialGradient2);
    painter.drawEllipse(QPoint(meteor_crater2_x,meteor_crater2_y),meteor_crater2_radius,meteor_crater2_radius);

    //陨石坑3
    painter.setBrush(radialGradient3);
    painter.drawEllipse(QPoint(meteor_crater3_x,meteor_crater3_y),meteor_crater3_radius,meteor_crater3_radius);
}
