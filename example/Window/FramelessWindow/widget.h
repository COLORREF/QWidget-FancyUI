#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QSpacerItem>
#include <QLineEdit>
#include <QCheckBox>
#include <QColorDialog>

namespace Ui {
class form;
class example;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void showExample(int index);
private:
    Ui::example *ui;
    QVector<Ui::form *> Uis;
};

#endif // WIDGET_H
