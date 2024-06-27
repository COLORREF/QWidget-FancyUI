#ifndef MOUSEEVENTCOLORMANAGEMENT_H
#define MOUSEEVENTCOLORMANAGEMENT_H

#include <QColor>
#include <QPen>
#include <QBrush>

#include "precompile_utils/precompile.h"

class MouseEventColorManagement
{
public:
    MouseEventColorManagement();
    MouseEventColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush);
    MouseEventColorManagement(const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen);
    MouseEventColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush,
                              const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen);

    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QBrush, normalBrush, NormalBrush,);        // QBrush _normalBrush;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QBrush, hoverBrush, HoverBrush,);          // QBrush _hoverBrush;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QBrush, pressHoverBrush, PressHoverBrush,) // QBrush _pressHoverBrush;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QBrush, pressLeaveBrush, PressLeaveBrush,) // QBrush _pressLeaveBrush;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QPen, normalPen, NormalPen,);              // QPen _normalPen;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QPen, hoverPen, HoverPen,);                // QPen _hoverPen;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QPen, pressHoverPen, PressHoverPen,)       // QPen _pressHoverPen;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(QPen, pressLeavePen, PressLeavePen,)       // QPen _pressLeavePen;
};

#endif // MOUSEEVENTCOLORMANAGEMENT_H
