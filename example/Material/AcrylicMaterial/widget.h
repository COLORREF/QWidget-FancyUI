#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class AcrylicMaterial;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent* event)override;
private:
    AcrylicMaterial* acrylicMaterial;
};
#endif // WIDGET_H
