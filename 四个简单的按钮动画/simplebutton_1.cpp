#include "simplebutton_1.h"

SimpleButton_1::SimpleButton_1(QWidget *parent):
    QPushButton(parent)
    ,timer(new QTimer(this))
{
    timer->setInterval(fill_speed);
    resize(width(),height());
}

void SimpleButton_1::setfillSpeed(const int speed)
{
    fill_speed = speed;
}

void SimpleButton_1::setFillColor(const QColor &fillcolor)
{
    fill_color = fillcolor;
}

void SimpleButton_1::setFontColor(const QColor &initial, const QColor &hover)
{
    font_initial_color = initial;
    font_hover_color = hover;
    font_paint_color = font_initial_color;
}

void SimpleButton_1::setText(const QString &text)
{
    QPushButton::setText("");
    this->text = text;
}

void SimpleButton_1::setBackgroundColor(const QColor &bk_color)
{
    setStyleSheet("QPushButton{background-color:" + bk_color.name() + ";}");
}

void SimpleButton_1::setBorderRadius(const int border_radius)
{
    this->border_radius = border_radius;
    path.clear();
    path.addRoundedRect(rect(), border_radius, border_radius);
    setStyleSheet(QString("QPushButton{border-radius:%1px;}").arg(QString::number(border_radius)));

}

void SimpleButton_1::setBorderColor(const QColor& border_color, const int border_thickness)
{
    setStyleSheet(QString("QPushButton{border:%1px solid %2;}").arg(QString::number(border_thickness), border_color.name()));
}

void SimpleButton_1::setStyleSheet(const QString &stylesheet)
{
    QPushButton::setStyleSheet(styleSheet() + stylesheet);
}

void SimpleButton_1::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipPath(path);

//    //绘制背景色
//    painter.setBrush(QBrush(Qt::black));
//    painter.drawRect(rect());
    //在resizeevent函数中使用样式表替代了

    //绘制填充圆
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(fill_color));
    painter.drawEllipse(QPoint(0,0), initial_radius, initial_radius);
    painter.drawEllipse(QPoint(width(),height()), initial_radius, initial_radius);

    //绘制文字
    painter.setFont(font());
    painter.setPen(font_paint_color);
    painter.drawText(rect(), Qt::AlignCenter, text);
}

void SimpleButton_1::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    max_radius = qPow(qPow(width(),2)+qPow(height(),2),0.5);
    path.clear();
    path.addRoundedRect(rect(), border_radius, border_radius);
    setBackgroundColor(Qt::black);
    setBorderRadius(border_radius);
}

void SimpleButton_1::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    font_paint_color = font_hover_color;
    timer->disconnect();
    connect(timer,&QTimer::timeout,this,RadiusIncrease);
    timer->start();
}

void SimpleButton_1::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    font_paint_color = font_initial_color;
    timer->disconnect();
    connect(timer,&QTimer::timeout,this,RadiusDecrease);
    timer->start();
}


