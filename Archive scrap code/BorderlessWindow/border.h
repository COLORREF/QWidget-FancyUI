#ifndef BORDER_H
#define BORDER_H

#include <QObject>
#include <QWidget>
#include <QWindow>
#include <QMouseEvent>
#include <QPainter>

enum class BorderType
{
    top_border = 1,
    bottom_border = 2,
    left_border = 3,
    right_border = 4,
    left_top_border = 5,
    right_top_border = 6,
    left_bottom_border = 7,
    right_bottom_border = 8
};

class Border : public QWidget
{
    Q_OBJECT
public:
    explicit Border(QWidget *parent, BorderType bordertype);

private:
    BorderType border_type;
    int border_width = 5;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent*event)override;
};

#endif // BORDER_H
