//
// Created by TX on 2025/8/31.
//

#include "RadioButton.h"
#include "RadioButtonStyle.h"

namespace fancy
{
    RadioButton::RadioButton(QWidget *parent) :
        QRadioButton(parent)
    {
        setStyle(new RadioButtonStyle(this));
        QFont font = this->font();
        font.setPointSize(12);
        setFont(font);
    }

    RadioButton::RadioButton(const QString &text, QWidget *parent) :
        RadioButton(parent)
    {
        setText(text);
    }
} // fancy
