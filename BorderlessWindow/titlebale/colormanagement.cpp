#include "colormanagement.h"

ColorManagement::ColorManagement()
{
}

ColorManagement::ColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush)
{
    this->normal_brush = normalBrush;
    this->hover_brush = hoverBrush;
    this->press_hover_brush = pressHoverBrush;
    this->press_leave_brush = pressLeaveBrush;
}

ColorManagement::ColorManagement(const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen)
{
    this->normal_pen = normalPen;
    this->hover_pen = hoverPen;
    this->press_hover_pen = pressHoverPen;
    this->press_leave_pen = pressLeavePen;
}

ColorManagement::ColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush, const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen)
{
    this->normal_brush = normalBrush;
    this->hover_brush = hoverBrush;
    this->press_hover_brush = pressHoverBrush;
    this->press_leave_brush = pressLeaveBrush;
    this->normal_pen = normalPen;
    this->hover_pen = hoverPen;
    this->press_hover_pen = pressHoverPen;
    this->press_leave_pen = pressLeavePen;
}

void ColorManagement::setNormalBrush(const QBrush &normalBrush)
{
    this->normal_brush = normalBrush;
}

void ColorManagement::setHoverBrush(const QBrush &hoverBrush)
{
    this->hover_brush = hoverBrush;
}

void ColorManagement::setPressHoverBrush(const QBrush &pressHoverBrush)
{
    this->press_hover_brush = pressHoverBrush;
}

void ColorManagement::setPressLeaveBrush(const QBrush &pressLeaveBrush)
{
    this->press_leave_brush = pressLeaveBrush;
}

void ColorManagement::setNormalPen(const QPen &normalPen)
{
    this->normal_pen = normalPen;
}

void ColorManagement::setHoverPen(const QPen &hoverPen)
{
    this->hover_pen = hoverPen;
}

void ColorManagement::setPressHoverPen(const QPen &pressHoverPen)
{
    this->press_hover_pen = pressHoverPen;
}

void ColorManagement::setPressLeavePen(const QPen &pressLeavePen)
{
    this->press_leave_pen = pressLeavePen;
}

const QBrush &ColorManagement::normalBrush() const
{
    return this->normal_brush;
}

const QBrush &ColorManagement::hoverBrush() const
{
    return this->hover_brush;
}

const QBrush &ColorManagement::pressHoverBrush() const
{
    return this->press_hover_brush;
}

const QBrush &ColorManagement::pressLeaveBrush() const
{
    return this->press_leave_brush;
}

const QPen &ColorManagement::normalPen() const
{
    return this->normal_pen;
}

const QPen &ColorManagement::hoverPen() const
{
    return this->hover_pen;
}

const QPen &ColorManagement::pressHoverPen() const
{
    return this->press_hover_pen;
}

const QPen &ColorManagement::pressLeavePen() const
{
    return this->press_leave_pen;
}

bool ColorManagement::isColorLight(const QColor &color)
{
    return (((5 * color.green()) + (2 * color.red()) + color.blue()) > (8 * 128));
}
