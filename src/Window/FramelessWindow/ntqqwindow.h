#ifndef NTQQWINDOW_H
#define NTQQWINDOW_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include "simpleframelesswindow.h"
// #include "windows.devices.lights.effects.h"
#include "lightanddarkwidget.h"
#include "precompile_utils/precompile.h"

class TitleBarBase;
class NTQQWindow : public SimpleFramelessWindow
{
    Q_OBJECT
public:
    explicit NTQQWindow(QWidget* parent = nullptr);
public slots:
    void setUnmaskSidebarColor(bool isUnmask = false);//取消侧边栏的遮罩颜色

    F_PRIVATE_POINTER_PUBLIC_GET(LightAndDarkWidget*,sidebar)//侧边栏
    F_PAINTEVENT;
};

#endif // NTQQWINDOW_H
