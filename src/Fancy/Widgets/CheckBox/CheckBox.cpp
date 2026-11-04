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
        setStyle(new CheckBoxStyle(this, smallIndicatorSize));
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
    }

    bool CheckBox::isAnimationEnabled() const
    {
        if (auto *checkBoxStyle = qobject_cast<CheckBoxStyle *>(style()))
            return checkBoxStyle->isAnimationEnabled();
        return false;
    }
} // fancy
