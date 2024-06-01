#ifndef TITLEBARBUTTON_H
#define TITLEBARBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QColor>
#include <QMouseEvent>
#include <QEvent>
#include <QEnterEvent>
#include <QLabel>
#include <QPainter>
#include "colormanagement.h"

class TitleBarButton : public QPushButton, public ColorManagement
{
    Q_OBJECT
public:
    // 鼠标状态
    enum class MouseState
    {
        NORMAL,
        HOVER,
        PRESSEDHOVER,
        PRESSEDLEAVE
    };
    explicit TitleBarButton(QWidget *parent);
    void setMouseState(TitleBarButton::MouseState state);
    bool isPressed();
    bool isHover();
    void setIcon(const QPixmap &icon);
    TitleBarButton::MouseState mouseState();
    void update();
    void setMouseTracking() = delete;

protected:
    MouseState mouse_state = MouseState::NORMAL;
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    virtual void enterEvent(QEnterEvent *event) override;
#else
    virtual void enterEvent(QEvent *event) override;
#endif
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *) override;

    // runtime color
    const QBrush &drawBrush();
    const QPen &drawPen();
signals:
    void mouseStateChange(MouseState);

private:
    QLabel *icon_label;
};

#endif // TITLEBARBUTTON_H
