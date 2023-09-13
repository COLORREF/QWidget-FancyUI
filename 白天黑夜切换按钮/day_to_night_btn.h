//注释和命名不规范，见谅
//完成时间：2023/09/13

#ifndef DAY_TO_NIGHT_BTN_H
#define DAY_TO_NIGHT_BTN_H

#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QPointF>
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QPainterPath>
#include <QLinearGradient>
#include <QGraphicsDropShadowEffect>
#include <QRadialGradient>

class day_to_night_frame;

class day_to_night_btn : public QPushButton
{
    Q_OBJECT

    friend class day_to_night_frame;
public:
    day_to_night_btn(QWidget* parent = nullptr);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:

    //关于坐标可以改成成QPoint或者QPointF
    QTimer* timer;
    int radius;         //圆角半径
    int sun_moon_x;     //太阳/月亮 圆心x（相对于按钮控件）
    int sun_moon_y;
    int sun_moon_S;     //太阳/月亮走过的路程
    int sun_moon_radius;//太阳/月亮半径

    //关于使用数组，不推荐。
    //原因：名称不直观，坐标位置需要微调，没有确定的关系，无法遍历赋值，（🐶括号写的头大

    int meteor_crater1_x;       //陨石坑1号圆心坐标
    int meteor_crater1_y;
    int meteor_crater1_radius;  //陨石坑1号半径

    int meteor_crater2_x;
    int meteor_crater2_y;
    int meteor_crater2_radius;

    int meteor_crater3_x;
    int meteor_crater3_y;
    int meteor_crater3_radius;

    int move_value;     //圆心坐标增量(平滑度，越小月平滑，大于0)
    int move_speed;     //移动速度(timer间隔)
    int A = 0;          //月亮透明度

    bool is_day = true;

};

#endif // DAY_TO_NIGHT_BTN_H
