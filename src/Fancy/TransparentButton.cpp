//
// Created by TX on 2025/9/30.
//

#include "TransparentButton.h"
#include "Icon.h"
#include "TransparentButtonStyle.h"

namespace fancy
{
    TransparentButton::TransparentButton(QWidget *parent) :
        QPushButton(parent)
    {
        setStyle(new TransparentButtonStyle(this));
        QFont font = this->font();
        font.setPixelSize(15);
        setFont(font);
    }

    TransparentButton::TransparentButton(const QString &text, QWidget *parent) :
        TransparentButton(parent)
    {
        setText(text);
    }

    TransparentButton::TransparentButton(const QIcon &icon, const QString &text, QWidget *parent) :
        TransparentButton(text, parent)
    {
        setIcon(icon);
    }

    TransparentButton::TransparentButton(const Icon &icon, const QString &text, QWidget *parent) :
        TransparentButton(text, parent)
    {
        setIcon(icon);
    }

    TransparentButton::TransparentButton(const IconId &id, const QString &text, QWidget *parent) :
        TransparentButton(Icon(id), text, parent) {}

    void TransparentButton::setDrawBorder(bool draw)
    {
        if (auto *style_ = dynamic_cast<TransparentButtonStyle *>(style()))
            style_->setDrawBorder(draw);
    }

    void TransparentButton::setRadius(int radius)
    {
        if (auto *style_ = dynamic_cast<TransparentButtonStyle *>(style()))
            style_->setRadius(radius);
    }

    void TransparentButton::setIcon(const IconId &id)
    {
        setIcon(Icon(id));
    }

    void TransparentButton::setIcon(const Icon &icon)
    {
        icon.stateOnUseAccentTextColor(true);
        QPushButton::setIcon(icon);
    }
} // fancy
