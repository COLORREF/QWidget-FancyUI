#ifndef HOVERFILLBUTTONBASE_H
#define HOVERFILLBUTTONBASE_H

#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <QTimer>
#include <QColor>
#include <QEvent>
#include <QBrush>
#include "custombuttonbase.h"

class HoverFillButtonBase : public CustomButtonBase
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
    explicit HoverFillButtonBase(QWidget *parent = Q_NULLPTR);
    explicit HoverFillButtonBase(const QString &text, QWidget *parent = nullptr);
    HoverFillButtonBase(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    void setFillSpeed(const int fillSpeed);     // 设置填充速度（设置定时器时间间隔，控制填充速度）
    void setFillBrush(const QBrush &fillBrush); // 设置填充颜色

protected:
    QTimer *timer = Q_NULLPTR;           // 定时器对象
    QBrush fill_brush{qRgb(255, 89, 0)}; // 填充画刷

    int max_painte_value;    // 最大绘制值
    int paint_variation = 2; // 绘制改变量
    int paint_value = 0;     // 绘制值(初始值为0)

    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

protected slots:
    virtual void paintValueReduction(); // 绘制值减小
    virtual void paintValueIncrease();  // 绘制值增大
private:
    void initializeMemberVariable();
};

#endif // HOVERFILLBUTTONBASE_H
