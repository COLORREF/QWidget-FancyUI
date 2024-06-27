#include "DynamicUnderlineLineEdit.h"

DynamicUnderlineLineEdit::DynamicUnderlineLineEdit(QWidget *parent)
    : QLineEdit{parent},
      timer{new QTimer(this)}
{
    initializeMemberVariable();
}
DynamicUnderlineLineEdit::DynamicUnderlineLineEdit(const QString &text, QWidget *parent)
    : QLineEdit::QLineEdit(text,parent)
{
    initializeMemberVariable();
}

void DynamicUnderlineLineEdit::setLinePen( const QPen &focusInPen,const QPen &focusOutPen)
{
    inactive_pen = focusOutPen;
    active_pen = focusInPen;
}

void DynamicUnderlineLineEdit::setTextColor(const QColor &textColor)
{
    const_cast<QPalette &>(palette()).setColor(QPalette::ColorRole::Text, textColor);
}

void DynamicUnderlineLineEdit::setPlaceholderTextColor(const QColor &placeholderText)
{
    const_cast<QPalette &>(palette()).setColor(QPalette::ColorRole::PlaceholderText, placeholderText);
}

void DynamicUnderlineLineEdit::setLineSpeed(int speed)
{
    timer->setInterval(speed);
}

void DynamicUnderlineLineEdit::initializeMemberVariable()
{
    setFocusPolicy(Qt::ClickFocus);
    timer->setInterval(12);
    connect(timer, &QTimer::timeout, this, &DynamicUnderlineLineEdit::inFocus);
    setAttribute(Qt::WA_TranslucentBackground);//背景透明
    setFrame(false);//无边框
    setTextMargins(10, 0, 0, 0);//设置文本左边距10像素
    inactive_pen.setColor(qRgb(66, 66, 66));
    inactive_pen.setWidth(2);
    active_pen.setColor(qRgb(0, 123, 255));
    active_pen.setWidth(2);
}

void DynamicUnderlineLineEdit::inFocus()
{
    right_coordinate += 10;
    if (right_coordinate > width())
        timer->stop();
    update();
}

void DynamicUnderlineLineEdit::outFocus()
{
    right_coordinate -= 10;
    if (right_coordinate < 0)
        timer->stop();
    update();
}

void DynamicUnderlineLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter painter = QPainter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing); // 抗锯齿
    painter.setPen(inactive_pen); // 设置画笔颜色和线条样式
    painter.drawLine(0, height() - inactive_pen.width(),width(), height() - inactive_pen.width()); // 在底部画未选中时的线条
    painter.setPen(active_pen);
    painter.drawLine(-2, height() - active_pen.width(),right_coordinate, height() - active_pen.width()); // 在底部画选中时的线条
}

void DynamicUnderlineLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, &DynamicUnderlineLineEdit::inFocus);
    timer->start();
}

void DynamicUnderlineLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, &DynamicUnderlineLineEdit::outFocus);
    timer->start();
}
