#include "hoverfillbutton.h"

HoverFillButton::HoverFillButton(QWidget *parent)
    : QPushButton(parent),
      timer(new QTimer(this)),
      fill_color(qRgb(255, 89, 0))
{
    timer->setInterval(timeInterval);                            // 设置定时器时间间隔
    max_radius = qSqrt(width() * width() + height() * height()); // 计算最大半径
    path.addRoundedRect(rect(), btn_radius, btn_radius);         // 为路径添加圆角矩形区域
}

HoverFillButton::HoverFillButton(const QString &text, QWidget *parent)
    :HoverFillButton::HoverFillButton(parent)
{
    QPushButton::setText(text);
}

void HoverFillButton::SetFillSpeed(const int fillSpeed)
{
    timer->setInterval(fillSpeed);
}

void HoverFillButton::setText(const QString &text, const QColor &fontcolor, const QFont &font)
{
    setFont(font);
    QPushButton::setText(text);
    const_cast<QPalette &>(palette()).setColor(QPalette::ButtonText, fontcolor); // 设置字体颜色
}

void HoverFillButton::setFillColor(const QColor &fillcolor)
{
    fill_color = fillcolor;
}

void HoverFillButton::setTextBehind(bool is_text_behind)
{
    this->is_text_behind = is_text_behind;
}

void HoverFillButton::addShadow(int right,int down, qreal blurRadius, const QColor &shadowColor)
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(right, down); //阴影的偏移量（右，下）
    shadow->setColor(shadowColor);  //阴影的颜色
    shadow->setBlurRadius(blurRadius); //控制阴影的模糊程度（光源距离）
    this->setGraphicsEffect(shadow);
}

void HoverFillButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    if (!mouse_point.isNull())
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);    // 设置抗锯齿
        painter.setBrush(QBrush(fill_color));             // 设置填充颜色
        painter.setPen(Qt::NoPen);                        // 设置无边框
        painter.setClipPath(path);                        // 绘制设置路径
        painter.drawEllipse(mouse_point, radius, radius); // 画圆
        if (!is_text_behind)                              // 如果没有设置文字在后面
        {
            painter.setFont(font());                                                // 设置字体
            painter.setPen(const_cast<QPalette &>(palette()).buttonText().color()); // 设置画笔颜色
            painter.drawText(rect(), Qt::AlignCenter, text());                      // 绘制文字
        }
    }
    return;
}

void HoverFillButton::enterEvent(QEnterEvent *event)
{
    mouse_point = event->position();             // 记录鼠标进入坐标
    timer->disconnect();                         // 断开可能的timer的所有连接
    connect(timer, &QTimer::timeout, this, [=] { // 定时器触发，半径增大
        radius += radius_var;
        update(); // 重绘
        if (radius > max_radius)
        {
            timer->stop(); // 达到最大半径，定时器停止
            return;
        }
    });
    timer->start();
}

void HoverFillButton::leaveEvent(QEvent *)
{
    mouse_point = mapFromGlobal(QCursor::pos());
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, [=] { // 定时器触发半径减小
        radius -= radius_var;
        update();
        if (radius <= 0)
        {
            timer->stop(); // 减小到小于0时定时器停止
            return;
        }
    });
    timer->start();
}

void HoverFillButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    max_radius = qSqrt(width() * width() + height() * height()); // 重新计算最大半径
    path.clear();
    path.addRoundedRect(rect(), btn_radius, btn_radius); // 重新为绘制路径添加圆角矩形区域
}
