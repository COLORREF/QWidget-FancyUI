//
// Created by TX on 2025/11/10.
//

#include "PlainTextEdit.h"

#include "Defs.h"
#include "palette.h"
#include "PlainTextEditStyle.h"
#include "ScrollBar.h"

namespace fancy
{
    PlainTextEdit::PlainTextEdit(QWidget *parent) :
        QPlainTextEdit(parent)
    {
        init();
    }

    PlainTextEdit::PlainTextEdit(const QString &text, QWidget *parent) :
        QPlainTextEdit(text, parent)
    {
        init();
    }

    void PlainTextEdit::onAppThemeChange()
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

    void PlainTextEdit::init()
    {
        setStyle(new PlainTextEditStyle(this));
        PlainTextEdit::onAppThemeChange();
        connect(&Palette::palette(), &Palette::appThemeChange, this, &PlainTextEdit::onAppThemeChange);
        setHorizontalScrollBar(new ScrollBar(Qt::Orientation::Horizontal, this));
        setVerticalScrollBar(new ScrollBar(Qt::Orientation::Vertical, this));
    }
} // fancy
