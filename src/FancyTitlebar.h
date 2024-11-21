#ifndef FANCYTITLEBAR_H
#define FANCYTITLEBAR_H

#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QColor>
#include <QMouseEvent>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWindow>
#include <QStyle>
#include <QFont>
#include <QApplication>
#include <QPalette>
#include <QPen>
#include <QBrush>
#include <QString>
#include "FancyTheme.h"
#include "precompile.h"
#include "FancyIcon.h"


class MouseEventColorManagement
{
public:
    MouseEventColorManagement(){};
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
    void setIcon(FancyIcon iconEnum, bool autoReverse = true);
    void setIcon(const QPicture& picture);
    void setSvgIcon(const QString& path);
    void setSvgIcon(const QByteArray& data);
    void setMouseTracking() = delete;

    F_PROTECTED_PROPERTY_PUBLIC_GETSET(MouseState,mouseState,MouseState,this->update();)
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(int,radius, Radius,)//圆角半径
    F_PROTECTED_POINTER_PUBLIC_GET(FancyIconWidget*,iconWidget)
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(bool,isClearBeforeNewPaint, IsClearBeforeNewPaint,)// 是否在新的绘制前清空旧的区域

protected slots:
    virtual void onThemeChange(Theme::Type themeType);
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

class MinimizeButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit MinimizeButton(QWidget *parent);

protected slots:
    void onThemeChange(Theme::Type themeType) override;
};

class MaximizeButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit MaximizeButton(QWidget *parent);
    enum class State
    {
        MAXIMIZE,
        NORMAL
    };
    MaximizeButton::State maxmizeState();

protected slots:
    void onThemeChange(Theme::Type themeType) override;
    void onStateChange(MaximizeButton::State state);

    // 最大化按钮无法获取到Qt消息,用以下函数处理
    void onEnter();
    void onLeave();
    void onMousePress();
    void onMouseRelease(bool isInSelf);
    void onMouseMove(bool isInSelf);

private:
    State state = State::NORMAL;

signals:
    void stateChange(MaximizeButton::State state);

    // 在nativeEvent中发射下面这些信号即可处理鼠标消息
    void enter();
    void leave();
    void mousePress();
    void mouseRelease(bool isInSelf);
    void mouseMove(bool isInSelf);
};

class CloseButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit CloseButton(QWidget *parent);
protected slots:
    void onThemeChange(Theme::Type) override;

    F_ENTEREVENT;
    F_LEAVEEVENT;
    F_MOUSEMOVEEVENT;
};

class TitleBarBase : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBarBase(QWidget *parent);

    F_PROTECTED_POINTER_PUBLIC_GET(CloseButton*, closeButton)
    F_PROTECTED_POINTER_PUBLIC_GET(QHBoxLayout *,horizontalLayout)
    F_PROTECTED_POINTER_PUBLIC_GET(QSpacerItem *,horizontalSpacer)

    F_MOUSEPRESSEVENT;
};

class SimpleTitleBar : public TitleBarBase
{
    Q_OBJECT
public:
    explicit SimpleTitleBar(QWidget *parent);

    F_MOUSEDOUBLECLICKEVENT;
    F_PROTECTED_POINTER_PUBLIC_GET(MinimizeButton*, minimizeButton);
    F_PROTECTED_POINTER_PUBLIC_GET(MaximizeButton*, maximizeButton);
};

class TitleTextLabel : public QLabel
{
    Q_OBJECT
public:
    using QLabel::QLabel;
    F_PAINTEVENT;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(bool,isClearBeforeNewPaint,IsClearBeforeNewPaint,)// 是否在新的绘制前清空旧的区域
};

class StandardTitleBar : public SimpleTitleBar
{
    Q_OBJECT
private:
    QColor _lightTitltTextColor{Qt::GlobalColor::black};
    QColor _darkTitltTextColor{Qt::GlobalColor::white};
    friend class TitleTextLabel;
public:
    explicit StandardTitleBar(QWidget *parent);
public slots:
    void setIcon(const QPixmap &icon);
    void setTitle(const QString &title);
    void setTitleFont(const QFont &font);
    void setTitleColor(const QColor &globalColor);
    void setTitleColor(const QColor &lightColor, const QColor &darkColor);

protected:
    virtual void onThemeChange(Theme::Type themeType);

    F_PROTECTED_POINTER_PUBLIC_GET(QLabel*,iconLabel)
    F_PROTECTED_POINTER_PUBLIC_GET(TitleTextLabel *,titleLabel)
};

#endif // FANCYTITLEBAR_H
