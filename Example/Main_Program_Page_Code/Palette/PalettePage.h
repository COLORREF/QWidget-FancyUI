//
// Created by TX on 2025/11/19.
//

#ifndef QWIDGET_FANCYUI_PALETTEPAGE_H
#define QWIDGET_FANCYUI_PALETTEPAGE_H
#include <QWidget>

class ExampleControlCard;
QT_BEGIN_NAMESPACE

namespace Ui
{
    class GeneralDisplayPage;
}

QT_END_NAMESPACE

class PalettePage : public QWidget
{
    Q_OBJECT

public:
    explicit PalettePage(QWidget *parent);

    ~PalettePage() override;

private:
    ExampleControlCard *makeCardLayout(const QString &introductionText, const QString &objName);

    Ui::GeneralDisplayPage *_ui;
    QList<ExampleControlCard *> _controlCards;
};


#endif //QWIDGET_FANCYUI_PALETTEPAGE_H
