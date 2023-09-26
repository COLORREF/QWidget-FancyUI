#include "DynamicLineEdit.h"

DynamicLineEdit::DynamicLineEdit(QWidget *parent):
    QLineEdit{parent},
    timer{new QTimer(this)}
{
    setFocusPolicy(Qt::ClickFocus);
    timer->setInterval(msec);
    connect(timer,&QTimer::timeout,this,Slot_in_focus);
    setStyleSheet(R"(DynamicLineEdit{
                        background-color:rgba(255,255,255,0);
                        border-radius:10px;
                        padding-left:10px;
                    })");
}

void DynamicLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    QPainterPath path = QPainterPath();
    path.addRoundedRect(rect(), border_radius, border_radius);  //添加圆角矩形区域

    QPainter painter = QPainter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);  //抗锯齿

    painter.setPen(QPen(line_color_1, line_height, Qt::SolidLine)); //设置画笔颜色和线条样式
    painter.drawLine(QPointF(0, height() - line_height), QPointF(width(), height() - line_height)); //在底部画未选中时的线条

    painter.setPen(QPen(line_color_2, line_height, Qt::SolidLine));
    painter.drawLine(QPointF(-2, height() - line_height), QPointF(line_width_anim, height() - line_height)); //在底部画选中时的线条
}

void DynamicLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    timer->disconnect();
    connect(timer,&QTimer::timeout,this,Slot_in_focus);
    timer->start();
}

void DynamicLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    timer->disconnect();
    connect(timer,&QTimer::timeout,this,Slot_out_focus);
    timer->start();
}


