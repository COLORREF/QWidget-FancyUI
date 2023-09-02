#include "mypushbutton_QFrame.h"

mypushbutton_QFrame::mypushbutton_QFrame(QWidget* parent)
    :QFrame(parent),timer(new QTimer(this)),
    fill_color(qRgb(255,89,0)),btn(new QPushButton(this))
{
    timer->setInterval(timeInterval);                                   //设置定时器时间间隔
    btn->resize(size());                                                //重设按钮大小与父类一样
    max_radius = qSqrt(width() * width() + height() * height());        //计算最大半径

    static QRegularExpression re("border-radius:\\s*(\\d+)px");         //正则获取圆角半径
    re.match(styleSheet()).hasMatch();
    btn_radius = re.match(styleSheet()).captured(1).toInt();
    //connect(btn,&QPushButton::clicked,this,[]{qDebug()<<"clicked";}); //qDeubg测试按钮功能
}

void mypushbutton_QFrame::setTimeInterval(const int timeinitval)
{
    timer->setInterval(timeinitval);
}

void mypushbutton_QFrame::setText(const QString& text, const QFont& font, const QColor& fontcolor)
{
    btn->setFont(font);
    btn->setText(text);
    btn->setStyleSheet("color: rgb("+
                       QString::number(fontcolor.red())+","+
                       QString::number(fontcolor.green())+","+
                       QString::number(fontcolor.blue())+")");  //设置字体颜色（此处为狗屎代码，可以自行优化）
}

void mypushbutton_QFrame::setFillColor(const QColor& fillcolor)
{
    fill_color = fillcolor;
}

void mypushbutton_QFrame::setStyleSheet(const QString& style)
{
    QFrame::setStyleSheet(style);
    static QRegularExpression re("border-radius:\\s*(\\d+)px");         //正则获取圆角半径
    re.match(styleSheet()).hasMatch();
    btn_radius = re.match(styleSheet()).captured(1).toInt();
}

void mypushbutton_QFrame::enterEvent(QEnterEvent* event)
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
        update();});                            //更新UI（作用类似刷新显示，（好像？大概是？🫤））
    timer->start();                             //定时器开始
}

void mypushbutton_QFrame::leaveEvent(QEvent*)
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

void mypushbutton_QFrame::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);
    if (!mouse_point.isNull())
    {
        QPainter painter(this);                             //创建画家对象，绘制窗口为this（也就是QFrame）
        QPainterPath path;                                  //创建绘制路径（区域（可以这么理解？🫤））
        painter.setRenderHint(QPainter::Antialiasing);      //设置抗锯齿
        painter.setBrush(QBrush(fill_color));               //设置填充颜色
        painter.setPen(Qt::NoPen);                          //设置无边框
        path.addRoundedRect(rect(),btn_radius,btn_radius);  //为路径添加圆角矩形区域（也就是QFrame的区域）
        painter.setClipPath(path);                          //设置路径
        painter.drawEllipse(mouse_point,radius,radius);     //画圆
    }
    return;
}

void mypushbutton_QFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    btn->resize(event->size());
    max_radius = qSqrt(width() * width() + height() * height());        //重新计算最大半径
}
