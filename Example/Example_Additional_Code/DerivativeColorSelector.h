//
// Created by TX on 2025/11/19.
//

#ifndef QWIDGET_FANCYUI_DERIVATIVECOLORSELECTOR_H
#define QWIDGET_FANCYUI_DERIVATIVECOLORSELECTOR_H
#include <QPushButton>

namespace fancy
{
    class TextLabel;
}

class QVariantAnimation;

class DerivativeColorButton : public QPushButton
{
    Q_OBJECT

public:
    explicit DerivativeColorButton(QWidget *parent);

    void setColor(const QColor &color);

    QColor color();

protected:
    void paintEvent(QPaintEvent *) override;

    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

private slots:
    void onAnimationChange(const QVariant &val);

private:
    QColor _color;
    QColor _textColor;
    QVariantAnimation *_ani;
    int _paintHeight; // 实际绘制区域高度
};


class DerivativeColorSelector : public QWidget
{
    Q_OBJECT

public:
    explicit DerivativeColorSelector(QWidget *parent, const QColor &baseColor, const QString &title);

    void setBaseColor(const QColor &color);

    void setTitleText(const QString &text);

private:
    fancy::TextLabel *_title;
    QList<DerivativeColorButton *> _buttons;
};


#endif //QWIDGET_FANCYUI_DERIVATIVECOLORSELECTOR_H
