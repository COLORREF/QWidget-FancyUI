#ifndef FANCYBUTTON_H
#define FANCYBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>
#include <QtMath>
#include <QPointF>
#include <QVariantAnimation>
#include <QPainter>
#include <QEnterEvent>
#include <QPainterPath>
#include <QVariant>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QFontMetrics>
#include "FancyIcon.h"


class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = nullptr);
    explicit PushButton(const QString &text, QWidget *parent = nullptr);
    PushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    PushButton(const Icon& icon, const QString &text, QWidget *parent = nullptr);
    PushButton(FancyIcon iconEnum, const QString &text, QWidget *parent = nullptr);
    void setCheckable(bool checkable);
    using QPushButton::setIcon;
    void setIcon(const Icon& icon);
    void setIcon(FancyIcon iconEnum);
};

class ThemeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ThemeButton(QWidget *parent = nullptr);
    explicit ThemeButton(const QString &text, QWidget *parent = nullptr);
    ThemeButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    ThemeButton(const Icon& icon, const QString &text, QWidget *parent = nullptr);
    ThemeButton(FancyIcon iconEnum, const QString &text, QWidget *parent = nullptr);
    using QPushButton::setIcon;
    void setIcon(const Icon &icon);
    void setIcon(FancyIcon iconEnum);
};

class RippleButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RippleButton(QWidget *parent = nullptr);
    explicit RippleButton(const QString &text, QWidget *parent = nullptr);
    RippleButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    RippleButton(const Icon& icon, const QString &text, QWidget *parent = nullptr);
    RippleButton(FancyIcon iconEnum, const QString &text, QWidget *parent = nullptr);
    using QPushButton::setIcon;
    void setIcon(const Icon &icon);
    void setIcon(FancyIcon iconEnum);
};

class TransparentButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TransparentButton(QWidget *parent = nullptr);
    explicit TransparentButton(const QString &text, QWidget *parent = nullptr);
    TransparentButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    TransparentButton(const Icon& icon, const QString &text, QWidget *parent = nullptr);
    TransparentButton(FancyIcon iconEnum, const QString &text, QWidget *parent = nullptr);
    void setCheckable(bool checkable);
    void preserveTransparency(bool preserve);
    using QPushButton::setIcon;
    void setIcon(const Icon &icon);
    void setIcon(FancyIcon iconEnum);  
};

class ToolButtonBase : public QToolButton
{
    Q_OBJECT
public:
    explicit ToolButtonBase(QWidget *parent = nullptr);
    void setCheckable(bool checkable);
    using QToolButton::setIcon;
    void setIcon(const Icon &icon);
    void setIcon(FancyIcon iconEnum);
};

class FillAnimationButton : public QPushButton
{
    Q_OBJECT
public:
    enum class AnimationType
    {
        CircularFill,      // 进入点圆形填充
        CrossFill,         // 左右两个小圆交叉填充
        DiagonalRectangle, // 斜着的矩形填充
        BottomCircle,      // 底部圆形填充
        LeftRectangle      // 左侧矩形填充
    };

    explicit FillAnimationButton(QWidget *parent = nullptr, AnimationType animationType = AnimationType::CircularFill);
    explicit FillAnimationButton(const QString &text, QWidget *parent = nullptr, AnimationType animationType = AnimationType::CircularFill);
    FillAnimationButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr, AnimationType animationType = AnimationType::CircularFill);

    void setAnimationType(AnimationType animationType);
    void setFillTime(int millisecond);                                              // 设置填充速度
    void setFillBrush(const QBrush &fillBrush);                                     // 设置填充颜色
    void setRadius(int radius);                                                     // 设置圆角半径
    void setTextColor(const QColor &normalTextColor, const QColor &hoverTextColor); // 设置文字未悬浮时颜色和悬浮时颜色
    void addShadow(int right, int down, qreal blurRadius, const QColor &shadowColor = Qt::black);
    void setBackgroundBrush(const QBrush &brush);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *) override;

private:
    QColor _hoverTextColor = Qt::black;         // 悬停时文字颜色
    QColor _normalTextColor = Qt::black;        // 普通状态下文字颜色
    QBrush _backgroundBrush = Qt::white;        // 背景笔刷
    bool _isCursorInside = false;               // 光标是否在内部
    int radius = 4;                             // 圆角半径
    AnimationType _animationType;               // 动画类型
    QPointF _mousePoint;                        // 鼠标离开和进入坐标
    QPainterPath _path;                         // 绘制路径
    QVariantAnimation *_animation = nullptr;    // 值动画，控制绘制半径
    QBrush _fillBrush{qRgb(255, 89, 0)};        // 填充画刷
    int _paintValue = 0;                        // 绘制值(初始值为0)
    void initializeMemberVariable(AnimationType animationType);
    void calculatedMaximumRadius();             // 计算最大半径
};

#endif // FANCYBUTTON_H
