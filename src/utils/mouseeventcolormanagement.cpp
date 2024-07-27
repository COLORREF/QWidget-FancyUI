#include "mouseeventcolormanagement.h"

MouseEventColorManagement::MouseEventColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush)
{
    this->_normalBrush = normalBrush;
    this->_hoverBrush = hoverBrush;
    this->_pressHoverBrush = pressHoverBrush;
    this->_pressLeaveBrush = pressLeaveBrush;
}

MouseEventColorManagement::MouseEventColorManagement(const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen)
{
    this->_normalPen = normalPen;
    this->_hoverPen = hoverPen;
    this->_pressHoverPen = pressHoverPen;
    this->_pressLeavePen = pressLeavePen;
}

MouseEventColorManagement::MouseEventColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush, const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen)
{
    this->_normalBrush = normalBrush;
    this->_hoverBrush = hoverBrush;
    this->_pressHoverBrush = pressHoverBrush;
    this->_pressLeaveBrush = pressLeaveBrush;
    this->_normalPen = normalPen;
    this->_hoverPen = hoverPen;
    this->_pressHoverPen = pressHoverPen;
    this->_pressLeavePen = pressLeavePen;
}
