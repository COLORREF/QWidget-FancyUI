#ifndef MSWINDOW_H
#define MSWINDOW_H

#include <QObject>
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include "transparenteffectwindowbase.h"
#include "lightanddarkwidget.h"
#include "precompile_utils/precompile.h"

class MSFilletedCornerWidget : public LightAndDarkWidget
{
public:
    using LightAndDarkWidget::LightAndDarkWidget;
    F_PAINTEVENT;
};

class MSWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit MSWindow(QWidget *parent = nullptr, WindowEffectType effectType = WindowEffectType::MICA);

    F_PRIVATE_PROPERTY(QHBoxLayout* ,horizontalLayout)
    F_PROTECTED_POINTER_PUBLIC_GET(MSFilletedCornerWidget*,clientArea)//客户区
    F_PROTECTED_POINTER_PUBLIC_GET(QWidget* ,sidebar)//侧边栏
};

#endif // MSWINDOW_H
