//
// Created by TX on 2025/11/19.
//

#include "DerivativeColorSelector.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QVariantAnimation>

#include "Defs.h"
#include "Palette.h"
#include "TextLabel.h"

DerivativeColorButton::DerivativeColorButton(QWidget *parent) :
    QPushButton(parent),
    _color(Qt::GlobalColor::white),
    _textColor(Qt::GlobalColor::black),
    _ani(new QVariantAnimation(this)),
    _paintHeight(40)
{
    setFixedSize(QSize(80, 55));

    // 鼠标进入时按钮绘制区域变高
    _ani->setDuration(150);
    _ani->setStartValue(40);
    _ani->setEndValue(55);
    connect(_ani, &QVariantAnimation::valueChanged, this, &DerivativeColorButton::onAnimationChange);
    // connect(this, &QPushButton::clicked, this, [this] { QGuiApplication::clipboard()->setText(_color.name()); });
}

void DerivativeColorButton::setColor(const QColor &color)
{
    _color = color.isValid() ? color : Qt::GlobalColor::white;
    setText(color.name());
    _textColor = fancy::Palette::apcaTextColor(color);
    update();
}

QColor DerivativeColorButton::color()
{
    return _color;
}

void DerivativeColorButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_color);

    const QRect r(0, height() - _paintHeight, width(), _paintHeight);
    painter.drawRoundedRect(r, 5, 5);

    // 在中心绘制颜色名称
    painter.setPen(_textColor);
    QFont font = painter.font();
    font.setPointSizeF(10);
    painter.setFont(font);
    painter.drawText(r, Qt::AlignCenter, text());
}

void DerivativeColorButton::enterEvent(QEnterEvent *event)
{
    _ani->setDirection(QAbstractAnimation::Direction::Forward);
    _ani->start();
}

void DerivativeColorButton::leaveEvent(QEvent *event)
{
    _ani->setDirection(QAbstractAnimation::Direction::Backward);
    _ani->start();
}

void DerivativeColorButton::onAnimationChange(const QVariant &val)
{
    _paintHeight = val.toInt();
    update();
}

DerivativeColorSelector::DerivativeColorSelector(QWidget *parent, const QColor &baseColor, const QString &title) :
    QWidget(parent),
    _title(new fancy::TextLabel(title, this))
{
    QList<QColor> colors = fancy::Palette::antDesign2ColorPalettes(baseColor);
    auto *wrap_widget = new QWidget(this);
    auto *wrap_layout = new QHBoxLayout(wrap_widget);
    wrap_layout->setSpacing(5);
    wrap_layout->setContentsMargins(0, 0, 0, 0);
    for (const QColor &color: colors)
    {
        auto *button = new DerivativeColorButton(this);
        button->setColor(color);
        wrap_layout->addWidget(button);
        _buttons.append(button);
    }

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    _title->setFont(QFont(_title->font().family(), 12));
    layout->addWidget(_title, 0, Qt::AlignHCenter);
    layout->addWidget(wrap_widget, 0, Qt::AlignHCenter);
}

void DerivativeColorSelector::setBaseColor(const QColor &color)
{
    QList<QColor> colors = fancy::Palette::antDesign2ColorPalettes(color);
    for (int i = 0; i < 10; i++)
        _buttons[i]->setColor(colors[i]);
}

void DerivativeColorSelector::setTitleText(const QString &text)
{
    _title->setText(text);
}
