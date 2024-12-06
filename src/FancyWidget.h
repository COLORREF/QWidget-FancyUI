#ifndef FANCYWIDGET_H
#define FANCYWIDGET_H


#include <QWidget>
#include <QSpacerItem>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QScroller>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include "precompile.h"

// 垂直布局惯性滚动区域，默认带有控件区域
class InertiaScrollArea : public QScrollArea
{
public:
    explicit InertiaScrollArea(QWidget *parent);
public:
    inline void addItem(QLayoutItem *item){this->_scrollContentWidgetVerticalLayout->addItem(item);}
    inline void insertItem(int index, QLayoutItem *item){this->_scrollContentWidgetVerticalLayout->insertItem(index, item);}
    inline void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment())
    {this->_scrollContentWidgetVerticalLayout->addWidget(widget, stretch, alignment);}
    inline void insertWidget(int index, QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment())
    {this->_scrollContentWidgetVerticalLayout->insertWidget(index, widget, stretch, alignment);}

    F_PROTECTED_PROPERTY(QWidget* ,scrollContentWidget)
    F_PROTECTED_PROPERTY(QVBoxLayout*, scrollContentWidgetVerticalLayout)
};


class AspectRatioImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AspectRatioImageWidget(QWidget* parent, bool keepCentered = true, int radius = 10);
    void setPixmap(const QPixmap& pixmap);
    void setRadius(int radius);
    void setKeepCentered(bool keepCentered);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap _pixmap;
    bool _keepCentered;
    int _radius;
};


#endif // FANCYWIDGET_H
