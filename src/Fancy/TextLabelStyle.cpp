//
// Created by TX on 2025/10/12.
//

#include "TextLabelStyle.h"
#include <QLabel>

#include "Defs.h"
#include "Palette.h"

namespace fancy
{
    TextLabelStyle::TextLabelStyle(QLabel *parent) :
        QProxyStyle(nullptr),
        _parent(parent)
    {
        setParent(parent);
        connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QLabel::update));
        connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QLabel::update));
    }

    void TextLabelStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
    {
        Q_UNUSED(flags)
        Q_UNUSED(pal)
        Q_UNUSED(textRole)
        painter->save();
        painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);
        Palette &palette = Palette::palette();
        painter->setPen(enabled ? palette[ColorRole::Text] : palette[ColorRole::DisEnabled]);
        painter->drawText(rect, _parent->alignment(), text);
        painter->restore();
    }
} // fancy
