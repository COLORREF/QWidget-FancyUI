#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <QList>
#include <QException>
#include <random>   //标准库（随机数库）

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

    //一个简单的随机数生成函数
    //参数：最小值,最大值,数量
    QList<int> UniformDist_int(const int minRange, const int maxRange, const int quantity = 1);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

};

#endif // WIDGET_H
