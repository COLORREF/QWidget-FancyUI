//
// Created by TX on 2025/11/14.
//

#include "ExampleControlCard.h"

#include <ostream>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QVBoxLayout>

#include "Defs.h"
#include "ExampleCodeCard.h"
#include "palette.h"
#include "ParallelAnimationGroupPool.h"
#include "TextLabel.h"
#include "ThemeModeController.h"

ExampleControlWidget::ExampleControlWidget(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void ExampleControlWidget::paintEvent(QPaintEvent *event)
{
    using namespace fancy;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    const bool isAppLight = ThemeModeController::controller().isAppLight();
    const QColor bkColor = isAppLight ? QColor(243, 243, 243) : QColor(32, 32, 32);
    const QColor borderColor = isAppLight ? QColor(234, 234, 234) : QColor(35, 35, 35);
    const QRect r = rect();
    QPainterPath path;
    path.moveTo(r.bottomLeft());
    path.lineTo(r.bottomRight());
    path.lineTo(r.topRight() + QPointF(0, ExampleCodeCard_Radius));
    path.quadTo(r.topRight(), r.topRight() - QPointF(ExampleCodeCard_Radius, 0));
    path.lineTo(r.topLeft() + QPointF(ExampleCodeCard_Radius, 0));
    path.quadTo(r.topLeft(), r.topLeft() + QPointF(0, ExampleCodeCard_Radius));
    path.closeSubpath();
    painter.fillPath(path, bkColor);
    painter.setClipPath(path);
    painter.strokePath(path, QPen(borderColor, 2));
}

ExampleControlCard::ExampleControlCard(QWidget *parent) :
    QWidget(parent),
    _control(new ExampleControlWidget(this)),
    _code(new ExampleCodeCard(this)),
    _arrow(new RotaryArrow(this)),
    _title(new fancy::TextLabel(this)),
    _layout(new QVBoxLayout(this))
{
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);
    _layout->addWidget(_control);
    _layout->addWidget(_code);
    _layout->addStretch(1);

    _arrow->setFixedSize(30, 30);
    _title->setFont(QFont(font().family(), 10));
    _title->setAttribute(Qt::WA_TransparentForMouseEvents, true);

    _code->_aniGroup->addAnimation(_arrow->_ani);

    connect(_arrow, &QPushButton::clicked, _code->_title, &QPushButton::click);
    connect(_code->_title,
            &QPushButton::clicked,
            this,
            [this] {
                if (_code->_title->_show)
                {
                    _arrow->_ani->setStartValue(180);
                    _arrow->_ani->setEndValue(360);
                }
                else
                {
                    _arrow->_ani->setStartValue(0);
                    _arrow->_ani->setEndValue(180);
                }
            }
    );
}

void ExampleControlCard::setTitleText(const QString &text)
{
    _title->setText(text);
}

void ExampleControlCard::setHtmlCode(const QString &html)
{
    _code->setHtmlCode(html);
}

QWidget *ExampleControlCard::controlAreaWidget()
{
    return _control;
}

void ExampleControlCard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    const QRect code = _code->geometry();
    _arrow->move(code.width() - 40, code.y() + ExampleCodeCard_ButtonHeight / 2 - _arrow->height() / 2);
    _title->move(code.x() + 20, code.y() + ExampleCodeCard_ButtonHeight / 2 - _title->height() / 2);
}
