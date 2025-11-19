//
// Created by TX on 2025/11/10.
//

#include "TextEdit.h"

#include "Defs.h"
#include "palette.h"
#include "ScrollBar.h"
#include "TextEditStyle.h"

namespace fancy
{
    TextEdit::TextEdit(QWidget *parent) :
        QTextEdit(parent)
    {
        init();
    }

    TextEdit::TextEdit(const QString &text, QWidget *parent) :
        QTextEdit(text, parent)
    {
        init();
    }

    void TextEdit::onAppThemeChange()
    {
        QPalette q_pal{palette()};
        auto &pal = Palette::palette();
        q_pal.setColor(QPalette::ColorRole::Text, pal[ColorRole::Text]);
        q_pal.setColor(QPalette::ColorRole::Highlight, pal[ColorRole::SysAccent]);
        q_pal.setColor(QPalette::ColorRole::HighlightedText, Palette::apcaTextColor(pal[ColorRole::SysAccent]));
        q_pal.setColor(QPalette::PlaceholderText, pal[ColorRole::PlaceholderText]);
        setPalette(q_pal);
        update();
    }

    void TextEdit::init()
    {
        setStyle(new TextEditStyle(this));
        TextEdit::onAppThemeChange();
        connect(&Palette::palette(), &Palette::appThemeChange, this, &TextEdit::onAppThemeChange);
        setHorizontalScrollBar(new ScrollBar(Qt::Orientation::Horizontal, this));
        setVerticalScrollBar(new ScrollBar(Qt::Orientation::Vertical, this));
    }
} // fancy
