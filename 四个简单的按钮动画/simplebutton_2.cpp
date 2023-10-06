#include "simplebutton_2.h"

SimpleButton_2::SimpleButton_2(QWidget *parent):
    QPushButton(parent),
    timer(new QTimer(this))
{
    timer->setInterval(fill_speed);
    resize(width(), height());
}

void SimpleButton_2::setfillSpeed(const int speed)
{
    fill_speed = speed;
}

void SimpleButton_2::setFillColor(const QColor& fillcolor)
{
    fill_color = fillcolor;
}

void SimpleButton_2::setFontColor(const QColor& initial, const QColor& hover)
{
    font_initial_color = initial;
    font_hover_color = hover;
}

void SimpleButton_2::setText(const QString& text)
{
    QPushButton::setText("");
    this->text = text;
}

void SimpleButton_2::setBackgroundColor(const QColor& bk_color)
{
    setStyleSheet("QPushButton{background-color:" + bk_color.name() + ";}");
}

void SimpleButton_2::setBorderRadius(const int border_radius)
{
    this->border_radius = border_radius;
    path.clear();
    path.addRoundedRect(rect(), border_radius, border_radius);
    setStyleSheet(QString("QPushButton{border-radius:%1px;}").arg(QString::number(border_radius)));

}

void SimpleButton_2::setBorderColor(const QColor& border_color, const int border_thickness)
{
    setStyleSheet(QString("QPushButton{border:%1px solid %2;}").arg(QString::number(border_thickness), border_color.name()));
}

void SimpleButton_2::setStyleSheet(const QString& stylesheet)
{
    QPushButton::setStyleSheet(styleSheet() + stylesheet);
}

void SimpleButton_2::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(fill_color));
    painter.setClipPath(path);

    painter.save();//保存状态
    //绘制矩形
    painter.setPen(Qt::NoPen);
    painter.translate(width() / 2, height() / 2);   //将坐标原点移动到按钮中心
    painter.rotate(45);                             //将坐标系旋转45度
    painter.drawRect(QRectF(-initial_width / 2, -width() / 2, initial_width, width()));

    painter.restore();//恢复状态
    //绘制文字
    painter.setFont(font());
    painter.setPen(font_paint_color);
    painter.drawText(rect(), Qt::AlignCenter, text);

}

void SimpleButton_2::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    max_width = qPow(qPow(width(), 2)+qPow(height(), 2), 0.5);
    path.clear();
    path.addRoundedRect(rect(), border_radius, border_radius);
    setBackgroundColor(Qt::black);
    setBorderRadius(border_radius);
}

void SimpleButton_2::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    font_paint_color = font_hover_color;
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, WidthIncrease);
    timer->start();
}

void SimpleButton_2::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    font_paint_color = font_initial_color;
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, WidthDecrease);
    timer->start();
}

