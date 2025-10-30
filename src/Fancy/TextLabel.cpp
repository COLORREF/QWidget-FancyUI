//
// Created by TX on 2025/9/19.
//

#include "TextLabel.h"
#include <QBrush>
#include <QPainter>

#include "TextLabelStyle.h"

namespace fancy
{
    TextLabel::TextLabel(QWidget *parent) :
        QLabel(parent),
        _clearBeforeNewPaint(false)
    {
        setStyle(new TextLabelStyle(this));
    }

    TextLabel::TextLabel(const QString &text, QWidget *parent) :
        TextLabel(parent)
    {
        setText(text);
    }

    void TextLabel::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::TextAntialiasing);
        if (_clearBeforeNewPaint)
        {
            painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Clear);
            painter.eraseRect(rect());
        }
        QLabel::paintEvent(event);
    }
} // fancy
