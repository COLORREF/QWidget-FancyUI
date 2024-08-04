#ifndef HOVERFILLBUTTON_H
#define HOVERFILLBUTTON_H

#include <QObject>
#include <QtMath>
#include <QPointF>
#include <QVariantAnimation>
#include <QPainter>
#include <QEnterEvent>
#include <QPainterPath>
#include <QPushButton>
#include <QWidget>
#include <QVariant>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QFontMetrics>

class HoverFillButton : public QPushButton
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

    explicit HoverFillButton(QWidget *parent = nullptr, AnimationType animationType = AnimationType::CircularFill);
    explicit HoverFillButton(const QString &text, QWidget *parent = nullptr, AnimationType animationType = AnimationType::CircularFill);
    HoverFillButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr, AnimationType animationType = AnimationType::CircularFill);

    void setAnimationType(AnimationType animationType);
    void setFillTime(int millisecond);                                              // 设置填充速度（设置定时器时间间隔，控制填充速度）
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
    QColor hover_text_color = Qt::black;    // 悬停时文字颜色
    QColor normal_text_color = Qt::black;   // 普通状态下文字颜色
    QBrush background_brush = Qt::white;    // 背景笔刷
    bool is_cursor_inside = false;          // 光标是否在内部
    int radius = 4;                         // 圆角半径
    AnimationType animation_type;           // 动画类型
    QPointF mouse_point;                    // 鼠标离开和进入坐标
    QPainterPath path;                      // 绘制路径
    QVariantAnimation *animation = nullptr; // 值动画，控制绘制半径
    QBrush fill_brush{qRgb(255, 89, 0)};    // 填充画刷
    int paint_value = 0;                    // 绘制值(初始值为0)
    void initializeMemberVariable(AnimationType animationType);
    void calculatedMaximumRadius(); // 计算最大半径
};



#endif // HOVERFILLBUTTON_H
