#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QColor>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
#include "mouseeventcolormanagement.h"
#include "theme.h"
#include "precompile_utils/precompile.h"

class TitleBarButton : public QPushButton, public MouseEventColorManagement
{
    Q_OBJECT
public:
    enum class MouseState
    {
        NORMAL,
        HOVER,
        PRESSEDHOVER,
        PRESSEDLEAVE
    };
    explicit TitleBarButton(QWidget *parent);
    bool isPressed();
    bool isHover();
    void setIcon(const QPixmap &icon);
    void update();
    void setMouseTracking() = delete;

    F_PROTECTED_PROPERTY_PUBLIC_GETSET(MouseState,mouseState,MouseState,this->update();)
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(int,radius, Radius,)//圆角半径
    F_PROTECTED_POINTER_PUBLIC_GET(QLabel*,iconLabel)
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(bool,isClearBeforeNewPaint, IsClearBeforeNewPaint,)// 是否在新的绘制前清空旧的区域

protected slots:
    virtual void onThemeChange(Theme::Type themeType) = 0;
protected:
    // runtime color
    const QBrush &drawBrush();
    const QPen &drawPen();

    F_ENTEREVENT;
    F_LEAVEEVENT;
    F_MOUSERELEASEEVENT;
    F_MOUSEPRESSEVENT;
    F_MOUSEMOVEEVENT;
    F_PAINTEVENT;
signals:
    void mouseStateChange(MouseState);
};

#endif // TITLEBARBUTTON_H
