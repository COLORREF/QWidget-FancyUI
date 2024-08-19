#include "pushbutton.h"
#include "buttonstylebase.h"

PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    this->setStyle(new ButtonStyleBase(this));
}

PushButton::PushButton(const QString &text, QWidget *parent)
    :QPushButton(text,parent)
{
    this->setStyle(new ButtonStyleBase(this));
}

PushButton::PushButton(const QIcon &icon, const QString &text, QWidget *parent)
    :QPushButton(icon,text,parent)
{
    this->setStyle(new ButtonStyleBase(this));
}

