#ifndef LIGHTANDDARKWIDGET_H
#define LIGHTANDDARKWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "themecolormanagement.h"
#include "precompile_utils/precompile.h"

class LightAndDarkWidget : public QWidget
{
    Q_OBJECT
public:
    LightAndDarkWidget(QWidget* parent,QColor light = QColor(248,249,252,127),QColor dark = QColor(255, 255, 255, 8));

    F_PAINTEVENT;
    F_PROTECTED_POINTER_PUBLIC_GET(ThemeColorManagement*, themeColorManagement)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(bool,isClearBeforeNewPaint,IsClearBeforeNewPaint,)// 是否在新的绘制前清空旧的区域
};

#endif // LIGHTANDDARKWIDGET_H
