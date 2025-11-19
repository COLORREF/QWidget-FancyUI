//
// Created by TX on 2025/11/10.
//

#ifndef QWIDGET_FANCYUI_EXAMPLECODECARD_H
#define QWIDGET_FANCYUI_EXAMPLECODECARD_H

#include <QPainterPath>
#include <QProxyStyle>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

#include "TransparentButton.h"

class QPropertyAnimation;
class QSvgRenderer;
class QVBoxLayout;
class QParallelAnimationGroup;
class QSpacerItem;
class QVariantAnimation;

namespace fancy
{
    class TransparentButton;
}

constexpr int ExampleCodeCard_ButtonHeight = 48;
constexpr int ExampleCodeCard_Radius = 8;

class RotaryArrow : public fancy::TransparentButton
{
    Q_OBJECT
    friend class ExampleCodeCard;
    friend class ExampleControlCard;

public:
    explicit RotaryArrow(QWidget *parent);

    void setArrowSideLength(int length);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onAnimationChanged(const QVariant &val);

private:
    QVariantAnimation *_ani;
    QList<QLineF> _lines;
    int _angle;
};

class ExampleCodeCardButton : public QPushButton
{
    Q_OBJECT
    friend class ExampleCodeCard;
    friend class ExampleControlCard;

public:
    explicit ExampleCodeCardButton(QWidget *parent);

    ExampleCodeCardButton(const QString &text, QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool _show;
    bool _transparent;
};

class ExampleCodeCardTextEditStyle : public QProxyStyle
{
    Q_OBJECT

public:
    explicit ExampleCodeCardTextEditStyle(QTextEdit *parent = nullptr);

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;
};

class ExampleCodeCardTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit ExampleCodeCardTextEdit(QWidget *parent);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void copyCode();

private:
    fancy::TransparentButton *_copy;
};

class ExampleCodeCard : public QWidget
{
    Q_OBJECT
    friend class ExampleControlCard;

public:
    explicit ExampleCodeCard(QWidget *parent);

    void setHtmlCode(const QString &html);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onAnimationChanged(const QVariant &val);

    void onAniFinished();

    void clickedToShowCode();

private:
    void updateHeight();

    ExampleCodeCardTextEdit *_code;
    ExampleCodeCardButton *_title;
    QVariantAnimation *_ani;
    QPropertyAnimation *_codeAni;
    QParallelAnimationGroup *_aniGroup;
};


#endif //QWIDGET_FANCYUI_EXAMPLECODECARD_H
