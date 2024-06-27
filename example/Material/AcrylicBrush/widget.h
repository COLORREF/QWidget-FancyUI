#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class AcrylicBrush;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent* event)override;
private:
    AcrylicBrush* acrylicBrush;
};
#endif // WIDGET_H
