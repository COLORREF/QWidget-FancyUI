//
// Created by TX on 2025/11/14.
//

#ifndef QWIDGET_FANCYUI_EXAMPLECONTROLCARD_H
#define QWIDGET_FANCYUI_EXAMPLECONTROLCARD_H

#include <QWidget>

#include "ExampleNavigationCard.h"

class RotaryArrow;
class QSpacerItem;
class QVBoxLayout;
class ExampleCodeCard;

class ExampleControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExampleControlWidget(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;
};

class ExampleControlCard : public QWidget
{
    Q_OBJECT

public:
    explicit ExampleControlCard(QWidget *parent);

    void setTitleText(const QString &text);

    void setHtmlCode(const QString &html);

    QWidget *controlAreaWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    ExampleControlWidget *_control;
    ExampleCodeCard *_code;
    RotaryArrow *_arrow;
    fancy::TextLabel *_title;
    QVBoxLayout *_layout;
};


#endif //QWIDGET_FANCYUI_EXAMPLECONTROLCARD_H
