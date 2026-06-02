//
// Created by TX on 2025/11/18.
//

#ifndef QWIDGET_FANCYUI_BASICINPUT_H
#define QWIDGET_FANCYUI_BASICINPUT_H
#include <QWidget>

class ExampleControlCard;
QT_BEGIN_NAMESPACE

namespace Ui
{
    class GeneralDisplayPage;
}

QT_END_NAMESPACE

class BasicInput : public QWidget
{
    Q_OBJECT

public:
    explicit BasicInput(QWidget *parent);

    ~BasicInput() override;

private:
    ExampleControlCard * makeCardLayout(const QString &introductionText,const QString& objName);


    Ui::GeneralDisplayPage *_ui;
    QList<ExampleControlCard *> _controlCards;
};


#endif //QWIDGET_FANCYUI_BASICINPUT_H
