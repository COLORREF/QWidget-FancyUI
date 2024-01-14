#include "hoverfillbuttonbase.h"

HoverFillButtonBase::HoverFillButtonBase(QWidget *parent)
    : CustomButtonBase(parent)
{
    initializeMemberVariable();
}

HoverFillButtonBase::HoverFillButtonBase(const QString &text, QWidget *parent)
    : CustomButtonBase(text, parent)
{
    initializeMemberVariable();
}

HoverFillButtonBase::HoverFillButtonBase(const QIcon &icon, const QString &text, QWidget *parent)
    : CustomButtonBase(icon, text, parent)
{
    initializeMemberVariable();
}

void HoverFillButtonBase::setFillSpeed(const int fillSpeed)
{
    timer->setInterval(fillSpeed);
}

void HoverFillButtonBase::setFillBrush(const QBrush &fillBrush)
{
    fill_brush = fillBrush;
}

void HoverFillButtonBase::enterEvent(QEnterEvent *event)
{
    CustomButtonBase::enterEvent(event);
    timer->disconnect(); // 断开可能的timer的所有连接
}

void HoverFillButtonBase::leaveEvent(QEvent *event)
{
    CustomButtonBase::leaveEvent(event);
    timer->disconnect(); // 断开可能的timer的所有连接
}

void HoverFillButtonBase::paintValueReduction()
{
    paint_value -= paint_variation;
    if (paint_value <= 0)
        timer->stop(); // 减小到0定时器停止
}

void HoverFillButtonBase::paintValueIncrease()
{
    paint_value += paint_variation;
    if (paint_value > max_painte_value)
        timer->stop(); // 达到最大值定时器停止
}

void HoverFillButtonBase::initializeMemberVariable()
{
    timer = new QTimer(this);
    timer->setInterval(10);
}
