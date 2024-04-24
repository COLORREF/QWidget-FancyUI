#ifndef MAINAREA_H
#define MAINAREA_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>
// #include <QGraphicsDropShadowEffect>
#include <QPainterPath>

class TitleBar;

class CustomerAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomerAreaWidget(QWidget *parent);
};

class MainArea : public QWidget
{
    Q_OBJECT
public:
    explicit MainArea(QWidget *parent);

    TitleBar *titleBar();

private:
    QVBoxLayout *vertical_layout = nullptr;
    TitleBar *title_bar = nullptr;
    CustomerAreaWidget *customer_area_widget = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MAINAREA_H
