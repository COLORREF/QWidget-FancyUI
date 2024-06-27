#ifndef MAINAREA_H
#define MAINAREA_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include "./titlebale/standardtitlebar.h"


class MainArea : public QWidget
{
    Q_OBJECT
public:
    explicit MainArea(QWidget *parent);
    QWidget *customerAreaWidget();
    StandardTitleBar *titleBar();

private:
    QVBoxLayout *vertical_layout = nullptr;
    StandardTitleBar *title_bar = nullptr;
    QWidget *customer_area_widget = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MAINAREA_H
