#include "day_to_night_frame.h"

day_to_night_frame::day_to_night_frame(QWidget *parent)
    :QFrame(parent)
    ,btn(new day_to_night_btn(this))
    ,timer(new QTimer(this))
    ,timer2(new QTimer(this))
    ,timer3(new QTimer(this))
{
    resize(width(),height());//调用resize，在resize事件中初始化

    connect(btn,&day_to_night_btn::clicked,this,[=](){
        timer->start();
        timer2->start();
        timer3->start();
        btn->setDisabled(true);
    });

    connect(timer,&QTimer::timeout,this,[=](){
        if(is_day)
        {
            sun_moon_x += move_value;
            if(A<255)
                A+=5;
        }
        else
        {
            sun_moon_x -= move_value;
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

    connect(timer2,&QTimer::timeout,this,[=](){
        if(is_day2)
        {
            white_circle2_x += move_value;
            cloud1_y += 2*move_value;
            cloud2_y += 2*move_value;
            cloud3_y += 2*move_value;
            cloud4_y += 2*move_value;
            cloud5_y += 2*move_value;
            cloud6_y += 2*move_value;
            under_cloude_path_dy +=2*move_value;
            star_dy += 2*move_value;
        }
        else
        {
            white_circle2_x -= move_value;
            cloud1_y -= 2*move_value;
            cloud2_y -= 2*move_value;
            cloud3_y -= 2*move_value;
            cloud4_y -= 2*move_value;
            cloud5_y -= 2*move_value;
            cloud6_y -= 2*move_value;
            under_cloude_path_dy -=2*move_value;
            star_dy -= 2*move_value;
        }
        if(white_circle2_x > (width()-const_white_circle2_x)&& is_day2)
        {
            is_day2 = false;
            timer2->stop();
        }
        else if(white_circle2_x < const_white_circle2_x&& !is_day2)
        {
            is_day2 = true;
            timer2->stop();
        }
        update();
    });

    connect(timer3,&QTimer::timeout,this,[=](){
        if(is_day3)
        {
            white_circle3_x += move_value;
        }
        else
        {
            white_circle3_x -= move_value;
        }

        if(white_circle3_x > (width()-const_white_circle3_x)&& is_day3)
        {
            is_day3 = false;
            timer3->stop();
            btn->setDisabled(false);
        }
        else if(white_circle3_x < const_white_circle3_x&& !is_day3)
        {
            is_day3 = true;
            timer3->stop();
            btn->setDisabled(false);
        }
        update();
    });
}

void day_to_night_frame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    radius = height()/2;//圆角半径
    sun_moon_radius = 0.81*radius;//太阳/月亮半径
    sun_moon_x = radius;//太阳/月亮圆心坐标
    sun_moon_y = radius;
    sun_moon_S = (width()-radius) - sun_moon_x;//太阳/月亮走过的路程

    //白云1
    cloud1_x = 1.1*sun_moon_x;
    cloud1_y = 2.53*sun_moon_y;
    cloud1_radius = 0.92*sun_moon_radius;

    //白云2
    cloud2_x = 1.9*sun_moon_x;
    cloud2_y = 2.4*sun_moon_y;
    cloud2_radius = 1.05*sun_moon_radius;

    //白云3
    cloud3_x = 2.79*sun_moon_x;
    cloud3_y = 2.13*sun_moon_y;
    cloud3_radius = 1.03*sun_moon_radius;

    //白云4
    cloud4_x = 3.64*sun_moon_x;
    cloud4_y = 2.19*sun_moon_y;
    cloud4_radius = 0.95*sun_moon_radius;

    //白云5
    cloud5_x = 4.33*sun_moon_x;
    cloud5_y = 1.72*sun_moon_y;
    cloud5_radius = 0.75*sun_moon_radius;

    //白云6
    cloud6_x = 4.97*sun_moon_x;
    cloud6_y = 1.37*sun_moon_y;
    cloud6_radius = 0.97*sun_moon_radius;

    //白圈半径
    white_circle_radius1 = 2*sun_moon_radius;
    white_circle_radius2 = ((sun_moon_x+3*sun_moon_radius)-(sun_moon_x-2*sun_moon_radius))/2;
    white_circle_radius3 = ((sun_moon_x+4*sun_moon_radius)-(sun_moon_x-2*sun_moon_radius))/2;

    //白圈圆心
    white_circle2_x = ((sun_moon_x-2*sun_moon_radius)+(sun_moon_x+3*sun_moon_radius))/2;
    white_circle3_x = ((sun_moon_x-2*sun_moon_radius)+(sun_moon_x+4*sun_moon_radius))/2;

    const_white_circle2_x = white_circle2_x;
    const_white_circle3_x = white_circle3_x;

    //白圈路程
    white_circle_S2 = (width()-const_white_circle2_x)-const_white_circle2_x;
    white_circle_S3 = (width()-const_white_circle3_x)-const_white_circle3_x;

    //星星绘制路径
    //临时变量
    int star_x = 2.21*sun_moon_x;
    int star_y = 1.4*sun_moon_y;
    int star_wh = 0.37*sun_moon_radius;
    QPainterPath r1;
    QPainterPath r2;
    QPainterPath r3;
    QPainterPath r4;
    //中等(3颗)
    QPainterPath sq1;//第1颗
    sq1.addRoundedRect(star_x,star_y,star_wh,star_wh,0,0);
    r1.addEllipse(star_x-star_wh/2,star_y-star_wh/2,star_wh,star_wh);
    r2.addEllipse(star_x+star_wh/2,star_y-star_wh/2,star_wh,star_wh);
    r3.addEllipse(star_x-star_wh/2,star_y+star_wh/2,star_wh,star_wh);
    r4.addEllipse(star_x+star_wh/2,star_y+star_wh/2,star_wh,star_wh);
    sq1 = sq1-r1-r2-r3-r4;
    sq1.translate(0, -6*sun_moon_radius);//向上平移
    QPainterPath sq2(sq1);//第2颗
    QPainterPath sq3(sq1);//第3颗
    sq2.translate(-0.83*sun_moon_x, -0.42*sun_moon_x);
    sq3.translate(-1.66*sun_moon_x, -0.83*sun_moon_x);

    //最大(2颗)
    //重设临时变量
    star_x = 2.64*sun_moon_x;
    star_y = 0.58*sun_moon_y;
    star_wh = 0.51*sun_moon_radius;
    r1.clear();
    r2.clear();
    r3.clear();
    r4.clear();
    QPainterPath sq4;//第1颗
    sq4.addRoundedRect(star_x,star_y,star_wh,star_wh,0,0);
    r1.addEllipse(star_x-star_wh/2,star_y-star_wh/2,star_wh,star_wh);
    r2.addEllipse(star_x+star_wh/2,star_y-star_wh/2,star_wh,star_wh);
    r3.addEllipse(star_x-star_wh/2,star_y+star_wh/2,star_wh,star_wh);
    r4.addEllipse(star_x+star_wh/2,star_y+star_wh/2,star_wh,star_wh);
    sq4 = sq4-r1-r2-r3-r4;
    sq4.translate(0,-6.2*sun_moon_radius);

    QPainterPath sq5(sq4);//第2颗
    sq5.translate(-1.53*sun_moon_x, -0.27*sun_moon_x);//平移

    //最小(6颗)
    star_x = 0.56*sun_moon_x;
    star_y = 1.4*sun_moon_y;
    star_wh = 0.22*sun_moon_radius;
    r1.clear();
    r2.clear();
    r3.clear();
    r4.clear();
    QPainterPath sq6;//1
    sq6.addRoundedRect(star_x,star_y,star_wh,star_wh,0,0);
    r1.addEllipse(star_x-star_wh/2,star_y-star_wh/2,star_wh,star_wh);
    r2.addEllipse(star_x+star_wh/2,star_y-star_wh/2,star_wh,star_wh);
    r3.addEllipse(star_x-star_wh/2,star_y+star_wh/2,star_wh,star_wh);
    r4.addEllipse(star_x+star_wh/2,star_y+star_wh/2,star_wh,star_wh);
    sq6 = sq6-r1-r2-r3-r4;
    sq6.translate(0,-5.9*sun_moon_radius);

    QPainterPath sq7(sq6);
    QPainterPath sq8(sq6);
    QPainterPath sq9(sq6);
    QPainterPath sq10(sq6);
    QPainterPath sq11(sq6);
    sq7.translate(0.2*sun_moon_x, -0.27*sun_moon_x);
    sq8.translate(0.68*sun_moon_x, 0.14*sun_moon_x);
    sq9.translate(1.31*sun_moon_x, -0.48*sun_moon_x);
    sq10.translate(1.51*sun_moon_x, -0.84*sun_moon_x);
    sq11.translate(2.07*sun_moon_x, -0.28*sun_moon_x);

    //合并路径，赋值给成员变量
    star_path = sq1+sq2+sq3+sq4+sq5+sq6+sq7+sq8+sq9+sq10+sq11;

    //下层白云绘制路径
    QPainterPath uc1;
    QPainterPath uc2;
    QPainterPath uc3;
    QPainterPath uc4;
    QPainterPath uc5;
    QPainterPath uc6;
    //微调坐标和大小形状
    uc1.addEllipse(QPointF(cloud1_x,0.7*cloud1_y),1.2*cloud1_radius,0.6*cloud1_radius);
    uc2.addEllipse(QPointF(cloud2_x,0.7*cloud2_y),cloud2_radius,cloud2_radius);
    uc3.addEllipse(QPointF(cloud3_x,0.7*cloud3_y),cloud3_radius,cloud3_radius);
    uc4.addEllipse(QPointF(cloud4_x,0.7*cloud4_y),cloud4_radius,cloud4_radius);
    uc5.addEllipse(QPointF(cloud5_x,0.6*cloud5_y),cloud5_radius,cloud5_radius);
    uc6.addEllipse(QPointF(cloud6_x,0.6*cloud6_y),cloud6_radius,cloud6_radius);
    under_cloude_path = uc1+uc2+uc3+uc4+uc5+uc6;

    //设置timer间隔(速度)
    setmove_speed(const_move_speed);
    timer->setInterval(2*move_speed/sun_moon_S);
    timer2->setInterval(2*move_speed/white_circle_S2);
    timer3->setInterval(2*move_speed/white_circle_S3);

    setStyleSheet("QFrame{border:none;border-radius:"+QString::number(radius)+"px;}");
    btn->setStyleSheet("QPushButton{"
                       "    border:none;"
                       "    border-radius:"+QString::number(radius)+"px;}");
    btn->radius = radius;
    btn->move_value = move_value;
    btn->resize(size());
}

void day_to_night_frame::paintEvent(QPaintEvent *event)
{  
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addRoundedRect(rect(),radius,radius);
    painter.setClipPath(path);//设置绘制路径

    //白天背景色
    painter.setBrush(QColor(70,133,192));
    painter.drawRoundedRect(rect(),radius,radius);

    //夜晚背景色
    painter.setBrush(QColor(25,30,50,A));
    painter.drawRoundedRect(rect(),radius,radius);

    //白圈1
    painter.setBrush(QColor(255,255,255,51));
    painter.drawEllipse(QPointF(sun_moon_x,sun_moon_y),white_circle_radius1,white_circle_radius1);

    //白圈2
    painter.setBrush(QColor(255,255,255,26));
    painter.drawEllipse(QPoint(white_circle2_x,sun_moon_y),white_circle_radius2,white_circle_radius2);

    //白圈3
    painter.setBrush(QColor(255,255,255,13));
    painter.drawEllipse(QPoint(white_circle3_x,sun_moon_y),white_circle_radius3,white_circle_radius3);

    //下层白云
    QPainterPath under_cloude_path_temp = under_cloude_path;
    under_cloude_path_temp.translate(0, under_cloude_path_dy);//向下平移
    painter.setBrush(QColor(255,255,255,100));
    painter.drawPath(under_cloude_path_temp);

    //上层白云1
    painter.setBrush(QColor(255,255,255,255));
    painter.drawEllipse(QPoint(cloud1_x,cloud1_y),cloud1_radius,cloud1_radius);

    //上层白云2
    painter.setBrush(QColor(255,255,255,255));
    painter.drawEllipse(QPoint(cloud2_x,cloud2_y),cloud2_radius,cloud2_radius);

    //上层白云3
    painter.setBrush(QColor(255,255,255,255));
    painter.drawEllipse(QPoint(cloud3_x,cloud3_y),cloud3_radius,cloud3_radius);

    //上层白云4
    painter.setBrush(QColor(255,255,255,255));
    painter.drawEllipse(QPoint(cloud4_x,cloud4_y),cloud4_radius,cloud4_radius);

    //上层白云5
    painter.setBrush(QColor(255,255,255,255));
    painter.drawEllipse(QPoint(cloud5_x,cloud5_y),cloud5_radius,cloud5_radius);

    //上层白云6
    painter.setBrush(QColor(255,255,255,255));
    painter.drawEllipse(QPoint(cloud6_x,cloud6_y),cloud6_radius,cloud6_radius);

    //星星
    QPainterPath star = star_path;
    star.translate(0, star_dy);//向下平移
    painter.setBrush(Qt::white);
    painter.drawPath(star);
}

double day_to_night_frame::getmove_speed()
{
    return const_move_speed;
}

void day_to_night_frame::setmove_speed(int time)
{
    move_speed = time;
    const_move_speed = move_speed;
    btn->move_speed = const_move_speed;
}

bool day_to_night_frame::Is_day()
{
    return is_day;
}


