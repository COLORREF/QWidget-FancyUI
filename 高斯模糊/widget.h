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
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

qreal Gaussian1D(qreal x,qreal o);


//QImage GaussiamBlur(qreal x, qreal o, QImage& img);



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


protected:
    virtual void paintEvent(QPaintEvent *event);
};


#endif // WIDGET_H

