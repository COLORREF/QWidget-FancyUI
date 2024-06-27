#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "day_to_night_frame.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QTimer* timer;
    int night_A = 0;//夜晚背景颜色透明度
    bool is_day = true;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

};
#endif // WIDGET_H
