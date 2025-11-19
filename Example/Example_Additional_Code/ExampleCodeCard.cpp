//
// Created by TX on 2025/11/10.
//

#include "ExampleCodeCard.h"

#include <QClipboard>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QVariantAnimation>
#include <QVBoxLayout>

#include "IconEngine.h"
#include "palette.h"
#include "ThemeModeController.h"
#include "TransparentButton.h"


RotaryArrow::RotaryArrow(QWidget *parent) :
    TransparentButton(parent),
    _ani(new QVariantAnimation(this)),
    _angle(0)
{
    _ani->setDuration(250);
    connect(_ani, &QVariantAnimation::valueChanged, this, &RotaryArrow::onAnimationChanged);
    connect(&fancy::Palette::palette(), &fancy::Palette::appThemeChange, this, QOverload<>::of(&RotaryArrow::update));
    connect(&fancy::Palette::palette(), &fancy::Palette::appColorChange, this, QOverload<>::of(&RotaryArrow::update));
    _lines = {{{-5.0, -2.5}, {0, 2.5}}, {{0, 2.5}, {5.0, -2.5}}};
}

void RotaryArrow::setArrowSideLength(int length)
{
    QPointF p1{-length / 2.0, -length / 4.0};
    QPointF p2{0, length / 4.0};
    QPointF p3{length / 2.0, -length / 4.0};
    _lines.clear();
    _lines.append({p1, p2});
    _lines.append({p2, p3});
}

void RotaryArrow::paintEvent(QPaintEvent *event)
{
    using namespace fancy;
    TransparentButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    const QPointF center{rect().width() / 2.0, rect().height() / 2.0};
    painter.translate(center);
    painter.rotate(_angle);
    painter.setPen(QPen(Palette::palette()[ColorRole::Text], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLines(_lines);
}

void RotaryArrow::onAnimationChanged(const QVariant &val)
{
    _angle = val.toInt();
    update();
}

ExampleCodeCardButton::ExampleCodeCardButton(QWidget *parent) :
    QPushButton(parent),
    _show(false),
    _transparent(true)
{
    connect(&fancy::Palette::palette(), &fancy::Palette::appThemeChange, this, QOverload<>::of(&ExampleCodeCardButton::update));
    connect(&fancy::Palette::palette(), &fancy::Palette::appColorChange, this, QOverload<>::of(&ExampleCodeCardButton::update));
}

ExampleCodeCardButton::ExampleCodeCardButton(const QString &text, QWidget *parent) :
    ExampleCodeCardButton(parent)
{
    setText(text);
}

void ExampleCodeCardButton::paintEvent(QPaintEvent *event)
{
    using namespace fancy;
    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    bool isAppLight = ThemeModeController::controller().isAppLight();
    const QColor bkColor = _transparent
                               ? isAppLight
                                     ? QColor(255, 255, 255, 70)
                                     : QColor(54, 54, 54, 87)
                               : isAppLight
                                     ? QColor(253, 253, 253)
                                     : QColor(50, 50, 50);
    const QColor borderColor = isAppLight ? QColor(234, 234, 234) : QColor(35, 35, 35);


    const QRect r = rect();
    QPainterPath path;
    if (!_show)
    {
        path.moveTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomRight() - QPointF(0, ExampleCodeCard_Radius));
        path.quadTo(r.bottomRight(), r.bottomRight() - QPointF(ExampleCodeCard_Radius, 0));
        path.lineTo(r.bottomLeft() + QPointF(ExampleCodeCard_Radius, 0));
        path.quadTo(r.bottomLeft(), r.bottomLeft() + QPointF(0, -ExampleCodeCard_Radius));
        path.closeSubpath();
    }
    else
        path.addRect(rect());

    painter.setClipPath(path);
    painter.fillPath(path, bkColor);
    painter.strokePath(path, QPen(borderColor, 2));
}

ExampleCodeCardTextEditStyle::ExampleCodeCardTextEditStyle(QTextEdit *parent) :
    QProxyStyle(nullptr)
{
    setParent(parent);
}

void ExampleCodeCardTextEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    using namespace fancy;
    if (element == PE_Frame)
    {
        painter->save();
        const bool isAppLight = ThemeModeController::controller().isAppLight();
        const QColor bkColor = isAppLight ? QColor(246, 246, 246, 70) : QColor(54, 54, 54, 87);
        const QColor borderColor = isAppLight ? QColor(234, 234, 234) : QColor(35, 35, 35);
        const QRect r = option->rect;
        QPainterPath path;
        // 上方直角，下方圆角
        path.moveTo(r.topLeft());
        path.lineTo(r.topRight());
        path.lineTo(r.bottomRight() - QPointF(0, ExampleCodeCard_Radius));
        path.quadTo(r.bottomRight(), r.bottomRight() - QPointF(ExampleCodeCard_Radius, 0));
        path.lineTo(r.bottomLeft() + QPointF(ExampleCodeCard_Radius, 0));
        path.quadTo(r.bottomLeft(), r.bottomLeft() + QPointF(0, -ExampleCodeCard_Radius));
        path.closeSubpath();
        painter->fillPath(path, bkColor);
        painter->strokePath(path, QPen(borderColor, 2));
        painter->restore();
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

ExampleCodeCardTextEdit::ExampleCodeCardTextEdit(QWidget *parent) :
    QTextEdit(parent),
    _copy(new fancy::TransparentButton(this))
{
    using namespace fancy;
    setStyle(new ExampleCodeCardTextEditStyle(this));

    setLineWrapMode(NoWrap);
    setWordWrapMode(QTextOption::NoWrap);

    connect(&Palette::palette(), &Palette::appThemeChange, this, QOverload<>::of(&ExampleCodeCardTextEdit::update));
    connect(&Palette::palette(), &Palette::appColorChange, this, QOverload<>::of(&ExampleCodeCardTextEdit::update));

    _copy->setFixedSize(32, 32);
    _copy->setIcon(IconPark::Copy);
    _copy->setDrawBorder(true);
    _copy->setBorderWidth(0.5);
    connect(_copy, &TransparentButton::clicked, this, &ExampleCodeCardTextEdit::copyCode);
}

void ExampleCodeCardTextEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    // _copy->move(width() - 10 - 32, height() / 2 - 32 / 2);
    _copy->move(width() - 42, height() / 2 - 16);
}

void ExampleCodeCardTextEdit::copyCode()
{
    QApplication::clipboard()->setText(toPlainText());
}

ExampleCodeCard::ExampleCodeCard(QWidget *parent) :
    QWidget(parent),
    _code(new ExampleCodeCardTextEdit(this)),
    _title(new ExampleCodeCardButton(this)),
    _ani(new QVariantAnimation(this)),
    _codeAni(new QPropertyAnimation(_code, "geometry", this)),
    _aniGroup(new QParallelAnimationGroup(this))
{
    _title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    _title->setFixedHeight(ExampleCodeCard_ButtonHeight);

    _code->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    _code->installEventFilter(this);
    _code->hide();
    _code->setReadOnly(true);

    _ani->setDuration(250);
    _codeAni->setDuration(250);
    _aniGroup->addAnimation(_ani);
    _aniGroup->addAnimation(_codeAni);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setFixedHeight(ExampleCodeCard_ButtonHeight);

    connect(_title, &QPushButton::clicked, this, &ExampleCodeCard::clickedToShowCode);
    connect(_ani, &QVariantAnimation::valueChanged, this, &ExampleCodeCard::onAnimationChanged);
    connect(_aniGroup, &QParallelAnimationGroup::finished, this, &ExampleCodeCard::onAniFinished);
}

void ExampleCodeCard::setHtmlCode(const QString &html)
{
    _code->setHtml(html);
}

void ExampleCodeCard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    _title->resize(width(), ExampleCodeCard_ButtonHeight);
    if (_title->_show)
        _code->resize(width(), _code->height());
}

void ExampleCodeCard::onAnimationChanged(const QVariant &val)
{
    int h = val.toInt();
    setFixedHeight(h);
}

void ExampleCodeCard::onAniFinished()
{
    _title->_show = !_title->_show;
    if (!_title->_show)
    {
        _code->document()->setTextWidth(-1); // 重置文本宽度
        _code->hide();
    }
    _title->_transparent = true;
    _title->update();
}

void ExampleCodeCard::clickedToShowCode()
{
    if (!_title->_show)
        _code->document()->setTextWidth(-1);
    updateHeight();

    const QSize codeSize{width(), _code->height()};
    if (_title->_show)
    {
        _ani->setStartValue(ExampleCodeCard_ButtonHeight + _code->height());
        _ani->setEndValue(ExampleCodeCard_ButtonHeight);
        constexpr QPoint startPos{0, ExampleCodeCard_ButtonHeight};

        const QPoint endPos{0, 0 - _code->height() - ExampleCodeCard_ButtonHeight};
        _codeAni->setStartValue(QRect{startPos, codeSize});
        _codeAni->setEndValue(QRect{endPos, codeSize});
    }
    else
    {
        _ani->setStartValue(ExampleCodeCard_ButtonHeight);
        _ani->setEndValue(ExampleCodeCard_ButtonHeight + _code->height());
        const QPoint startPos{0, 0 - _code->height() - ExampleCodeCard_ButtonHeight};
        constexpr QPoint endPos{0, ExampleCodeCard_ButtonHeight};
        _codeAni->setStartValue(QRect{startPos, codeSize});
        _codeAni->setEndValue(QRect{endPos, codeSize});
    }
    if (_aniGroup->state() != QAbstractAnimation::State::Running)
    {
        _aniGroup->start();
        _code->show();
        _title->_transparent = false;
    }
}

void ExampleCodeCard::updateHeight()
{
    _code->document()->adjustSize();
    const int h = qRound(_code->document()->size().height())
                  + _code->contentsMargins().top()
                  + _code->contentsMargins().bottom()
                  + qRound(_code->document()->documentMargin() * 2);
    _code->setFixedHeight(h);
}
