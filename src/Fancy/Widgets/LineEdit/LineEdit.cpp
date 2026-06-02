//
// Created by TX on 2025/11/10.
//

#include "LineEdit.h"

#include "LineEditStyle.h"
#include "Core/Defs.h"
#include "Core/Palette/Palette.h"

namespace fancy
{
    LineEdit::LineEdit(QWidget *parent) :
        QLineEdit(parent)
    {
        setStyle(new LineEditStyle(this));
        LineEdit::onAppThemeChange();
        connect(&Palette::palette(), &Palette::appThemeChange, this, &LineEdit::onAppThemeChange);
    }

    LineEdit::LineEdit(const QString &text, QWidget *parent) :
        LineEdit(parent)
    {
        setText(text);
    }

    void LineEdit::onAppThemeChange()
    {
        QPalette q_pal{palette()};
        auto &pal = Palette::palette();
        q_pal.setColor(QPalette::ColorRole::Text, pal[ColorRole::Text]);
        q_pal.setColor(QPalette::ColorRole::Highlight, pal[ColorRole::SysAccent]);
        q_pal.setColor(QPalette::ColorRole::HighlightedText, Palette::apcaTextColor(pal[ColorRole::SysAccent]));
        q_pal.setColor(QPalette::PlaceholderText, pal[ColorRole::PlaceholderText]);
        setPalette(q_pal);
    }
} // fancy
