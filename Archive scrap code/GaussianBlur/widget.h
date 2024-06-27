#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <qmath.h>
#include <QImage>
#include <QPainter>
#include <QRgb>
#include <QVector>
#include <algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

void GaussiamBlur(int r, qreal variance, QImage& img);



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QImage img;
    QImage constimg;

    int r = 0;//模糊半径
    qreal o = 0.01;//方差


protected:
    virtual void paintEvent(QPaintEvent *event);
};


#endif // WIDGET_H

