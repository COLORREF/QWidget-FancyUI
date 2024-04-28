#include "RippleEffect.h"

RippleEffect::RippleEffect(QWidget* parent)
    :QFrame(parent),timer(new QTimer(this)),
    fill_color(qRgb(255,89,0))
{
    timer->setInterval(timeInterval);                                   //设置定时器时间间隔
    max_radius = qSqrt(width() * width() + height() * height());        //计算最大半径
    setStyleSheet("*{border:none;}");                                   //去除默认边框和默认的特效
}

void RippleEffect::setSpeed(const int timeinitval)
{
    timer->setInterval(timeinitval);
}

void RippleEffect::setFillColor(const QColor& fillcolor)
{
    fill_color = fillcolor;
}

void RippleEffect::setRadius(int radius_)
{
    frame_radius = radius_;
    setStyleSheet("*{border:none;border-radius:15px;}");   //圆角
}

void RippleEffect::enterEvent(QEnterEvent* event)
{
    mouse_point = event->position();            //记录鼠标进入坐标
    timer->disconnect();                        //断开可能的timer的所有连接
    connect(timer, &QTimer::timeout, this, [=]{ //定时器触发，半径增大
        radius += radius_var;
        if (radius > max_radius)
        {
            timer->stop();                      //达到最大半径，定时器停止
            return;
        }
        update();});                            //调用绘制事件
    timer->start();                             //定时器开始
}

void RippleEffect::leaveEvent(QEvent*)
{                                               //类似enterEvent
    mouse_point = mapFromGlobal(QCursor::pos());
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, [=]{ //定时器触发半径减小
        radius -= radius_var;
        if (radius < 0)
        {
            timer->stop();                      //减小到小于0时定时器停止
            return;
        }
        update();});
    timer->start();
}

void RippleEffect::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);
    if (!mouse_point.isNull() && radius>0)
    {
        QPainter painter(this);                                //创建画家对象，绘制窗口为this（也就是QFrame）
        QPainterPath path;                                     //创建绘制路径（区域）
        painter.setRenderHint(QPainter::Antialiasing);         //设置抗锯齿
        painter.setBrush(QBrush(fill_color));                  //设置填充颜色
        painter.setPen(Qt::NoPen);                             //设置无边框
        path.addRoundedRect(rect(),frame_radius,frame_radius); //为路径添加圆角矩形区域（也就是QFrame的区域）
        painter.setClipPath(path);                             //设置路径
        painter.drawEllipse(mouse_point,radius,radius);        //画圆
    }
    return;
}

void RippleEffect::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    max_radius = qSqrt(width() * width() + height() * height());        //重新计算最大半径
}
