#include "radiobutton.h"
#include "radiobuttonstyle.h"

RadioButton::RadioButton(QWidget *parent) : QRadioButton(parent)
{
    QRadioButton::setStyle(new RadioButtonStyle(this));
}

RadioButton::RadioButton(const QString &text, QWidget *parent)
    : RadioButton(parent)
{
    QRadioButton::setText(text);
}
