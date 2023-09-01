#ifndef DYNAMICBORDERFRAME_H
#define DYNAMICBORDERFRAME_H
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

class DynamicBorderFrame : public QFrame
{
    Q_OBJECT

public:
    DynamicBorderFrame(QWidget* parent = Q_NULLPTR);
    ~DynamicBorderFrame();

    void setBackgroundColor(const QColor& bkcolor);
    void setBorderRadius(int radius_);
    int getBorderRadius();
    void setDynamicColor(const QBrush& dycolor1 = Qt::cyan, const QBrush& dycolor2 = Qt::cyan);

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    int angle = 0;
    QTimer* timer = Q_NULLPTR;
    int timeInterval = 10;
    QColor backgroundcolor = QColor(0, 0, 0, 0);
    int radius = 0;
    QBrush dynamic_color[2] = {Qt::cyan, Qt::cyan};

    void setStyleSheet(const QString& style);
};

#endif // DYNAMICBORDERFRAME_H
