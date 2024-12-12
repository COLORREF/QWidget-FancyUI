#include "FancyRadiobutton.h"
#include "FancyStyle.h"

RadioButton::RadioButton(QWidget *parent) : QRadioButton(parent)
{
    this->setStyle(new RadioButtonStyle(this));
}

RadioButton::RadioButton(const QString &text, QWidget *parent)
    : RadioButton(parent)
{
    setText(text);
}

void RadioButton::setText(const QString &text)
{
    if(!(text.isNull() || text.isEmpty()))
    {
        _userSetNullText = false;
        _userSetEmptyText = false;
        QRadioButton::setText(text);
        return;
    }
    else if(text.isNull())
    {
        _userSetNullText = true;
        _userSetEmptyText = false;
    }
    else if(text.isEmpty())
    {
        _userSetNullText = false;
        _userSetEmptyText = true;
    }
    QRadioButton::setText(QString(" "));
}

QString RadioButton::text() const
{
    if(_userSetNullText)
        return QString();
    else if(_userSetEmptyText)
        return QString("");
    return QRadioButton::text();
}

void RadioButton::enableJumpAnimation(bool isEnable)
{
    auto style = dynamic_cast<RadioButtonStyle*>(this->style());
    if(style)
        style->enableJumpAnimation(isEnable);
}

