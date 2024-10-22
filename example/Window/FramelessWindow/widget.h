#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QMap>
#include <QSpacerItem>
#include <QLineEdit>
#include <QCheckBox>
#include <QColorDialog>

#if (QT_VERSION <= QT_VERSION_CHECK(6, 3, 0))
#include <QRegularExpression>
#endif

namespace Ui
{
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
