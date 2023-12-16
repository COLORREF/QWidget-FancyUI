#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QPaintEvent>


QT_BEGIN_NAMESPACE
namespace Ui {class Widget;}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    void Areo();

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void changeColor();
};
#endif // WIDGET_H
