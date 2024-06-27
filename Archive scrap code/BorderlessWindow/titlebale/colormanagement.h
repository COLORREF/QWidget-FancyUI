#ifndef COLORMANAGEMENT_H
#define COLORMANAGEMENT_H

#include <QColor>
#include <QPen>
#include <QBrush>

class ColorManagement
{
public:
    ColorManagement();
    ColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush);
    ColorManagement(const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen);
    ColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush,
                    const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen);

    void setNormalBrush(const QBrush &normalBrush);
    void setHoverBrush(const QBrush &hoverBrush);
    void setPressHoverBrush(const QBrush &pressHoverBrush);
    void setPressLeaveBrush(const QBrush &pressLeaveBrush);

    void setNormalPen(const QPen &normalPen);
    void setHoverPen(const QPen &hoverPen);
    void setPressHoverPen(const QPen &pressHoverPen);
    void setPressLeavePen(const QPen &pressLeavePen);

    const QBrush &normalBrush() const;
    const QBrush &hoverBrush() const;
    const QBrush &pressHoverBrush() const;
    const QBrush &pressLeaveBrush() const;
    const QPen &normalPen() const;
    const QPen &hoverPen() const;
    const QPen &pressHoverPen() const;
    const QPen &pressLeavePen() const;
    static bool isColorLight(const QColor &color);

protected:
    QBrush normal_brush;
    QBrush hover_brush;
    QBrush press_hover_brush;
    QBrush press_leave_brush;

    QPen normal_pen;
    QPen hover_pen;
    QPen press_hover_pen;
    QPen press_leave_pen;
};

#endif // COLORMANAGEMENT_H
