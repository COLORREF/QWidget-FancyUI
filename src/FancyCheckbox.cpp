#include "FancyCheckbox.h"
#include "FancyStyle.h"

CheckBox::CheckBox(QWidget *parent)
    :QCheckBox(parent)
{
    this->setStyle(new CheckBoxStyle(this));
    this->style()->setParent(this);
}

CheckBox::CheckBox(const QString &text, QWidget *parent)
    :CheckBox(parent)
{
    setText(text);
}

void CheckBox::setText(const QString &text)
{
    if(!(text.isNull() || text.isEmpty()))
    {
        _userSetNullText = false;
        _userSetEmptyText = false;
        QCheckBox::setText(text);
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
    QCheckBox::setText(QString(" "));
}

QString CheckBox::text() const
{
    if(_userSetNullText)
        return QString();
    else if(_userSetEmptyText)
        return QString("");
    return QCheckBox::text();
}

