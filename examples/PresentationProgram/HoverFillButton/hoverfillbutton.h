#ifndef HOVERFILLBUTTON_H
#define HOVERFILLBUTTON_H

#include <QObject>
#include <QtMath>
#include <QPointF>
#include <QTimer>
#include <QPainter>
#include <QEnterEvent>
#include "hoverfillbuttonbase.h"

class HoverFillButton : public HoverFillButtonBase
{
    Q_OBJECT
public:
    explicit HoverFillButton(QWidget *parent = Q_NULLPTR, AnimationType animationType = AnimationType::CircularFill);
    explicit HoverFillButton(const QString &text, QWidget *parent = Q_NULLPTR, AnimationType animationType = AnimationType::CircularFill);
    HoverFillButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr, AnimationType animationType = AnimationType::CircularFill);

    void setAnimationType(AnimationType animationType);

private:
    AnimationType animation_type; // 动画类型
    QPointF mouse_point;          // 鼠标离开和进入坐标

protected slots:
    void paintValueReduction() override; // 绘制值减小
    void paintValueIncrease() override;  // 绘制值增大

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    inline void initializeMemberVariable();
};

#endif // HOVERFILLBUTTON_H
