//
// Created by TX on 2025/8/30.
//

#include "CheckBox.h"
#include "CheckBoxStyle.h"

namespace fancy
{
    CheckBox::CheckBox(QWidget *parent, bool smallIndicatorSize) :
        QCheckBox(parent)
    {
        smallIndicatorSize ? setStyle(new CheckBoxStyle_SM(this)) : setStyle(new CheckBoxStyle(this));
        QFont font = this->font();
        font.setPointSize(smallIndicatorSize ? 10 : 12);
        setFont(font);
    }

    CheckBox::CheckBox(const QString &text, QWidget *parent, bool smallIndicatorSize) :
        CheckBox(parent, smallIndicatorSize)
    {
        setText(text);
    }

    void CheckBox::setAnimationEnabled(bool enabled)
    {
        if (auto *checkBoxStyle = qobject_cast<CheckBoxStyle *>(style()))
            checkBoxStyle->setAnimationEnabled(enabled);
        else if (auto *smallCheckBoxStyle = qobject_cast<CheckBoxStyle_SM *>(style()))
            smallCheckBoxStyle->setAnimationEnabled(enabled);
    }

    bool CheckBox::isAnimationEnabled() const
    {
        if (auto *checkBoxStyle = qobject_cast<CheckBoxStyle *>(style()))
            return checkBoxStyle->isAnimationEnabled();
        if (auto *smallCheckBoxStyle = qobject_cast<CheckBoxStyle_SM *>(style()))
            return smallCheckBoxStyle->isAnimationEnabled();
        return false;
    }
} // fancy
