#ifndef DYNAMICBOUNDING_H
#define DYNAMICBOUNDING_H
#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QFont>
#include <QPixmap>
#include <QColor>
#include <QConicalGradient>
#include <QMouseEvent>
#include <QPushButton>
#include <QRegularExpression>
#include <QBrush>
#include <QLayout>
#include <QScopedPointer>
#include <QList>

class DynamicBounding : public QFrame
{
public:
    DynamicBounding(QWidget* parent = Q_NULLPTR);
    ~DynamicBounding();

    void setBackgroundColor(const QColor& bkcolor);
    void setBorderRadius(int radius_);
    int getBorderRadius();
    void setDynamicColor(const QBrush& dycolor1 = Qt::cyan, const QBrush& dycolor2 = Qt::cyan);
    void setStyleSheet(const QString& style);
    void setSpeed(int time = 10);
protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    int angle = 0;
    QTimer* timer = Q_NULLPTR;
    int timeInterval = 10;
    QColor backgroundcolor = QColor(0, 0, 0, 0);
    int radius = 0;
    QBrush dynamic_color[2] = {Qt::cyan, Qt::cyan};


};
#endif // DYNAMICBOUNDING_H
