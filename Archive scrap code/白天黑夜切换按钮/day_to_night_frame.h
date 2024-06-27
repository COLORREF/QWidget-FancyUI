//注释和命名不规范，见谅
//完成时间：2023/09/13

#ifndef DAY_TO_NIGHT_FRAME_H
#define DAY_TO_NIGHT_FRAME_H

#include <QFrame>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include "day_to_night_btn.h"

class day_to_night_frame : public QFrame
{
    Q_OBJECT
public:
    day_to_night_frame(QWidget* parent = nullptr);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

public:
    day_to_night_btn* btn = nullptr;
private:

    //减少Qtimer可以提高性能，但需要修改代码，代码会变的异常复杂，而且动态效果也不好，（也可能是我能力不足）
    QTimer* timer;
    QTimer* timer2;
    QTimer* timer3;
    int radius;     //圆角半径

    int sun_moon_x; //太阳/月亮 圆心x
    int sun_moon_y;

    int sun_moon_S; //太阳/月亮走过的路程

    //关于使用数组，不推荐。
    //原因：名称不直观，坐标位置需要微调，没有确定的关系，无法遍历赋值，（括号写的头大

    //上层白云1
    int cloud1_x;
    int cloud1_y;
    int cloud1_radius;

    //上层白云2
    int cloud2_x;
    int cloud2_y;
    int cloud2_radius;

    //上层白云3
    int cloud3_x;
    int cloud3_y;
    int cloud3_radius;

    //上层白云4
    int cloud4_x;
    int cloud4_y;
    int cloud4_radius;

    //上层白云5
    int cloud5_x;
    int cloud5_y;
    int cloud5_radius;

    //上层白云6
    int cloud6_x;
    int cloud6_y;
    int cloud6_radius;

                        //白圈1坐标和太阳月亮一样,忽略
    int white_circle2_x;//白圈2 圆心x(会随timer变化)
    int white_circle3_x;//白圈3 圆心x(会随timer变化)

    int const_white_circle2_x;//白圈2 圆心x(不随timer变化)
    int const_white_circle3_x;//白圈3 圆心x(不随timer变化)

    int white_circle_S2;//白圈2走过的路程
    int white_circle_S3;//白圈3走过的路程

    int sun_moon_radius;//太阳/月亮半径
    int white_circle_radius1;//白圈1半径
    int white_circle_radius2;//白圈2半径
    int white_circle_radius3;//白圈2半径

    int move_value = 2; //圆心坐标增量(平滑度，越小月平滑，大于0)
    int move_speed;//移动速度(timer间隔)
    int const_move_speed = 600;//备份移动速度，在控件改变大小时可以维持原来的移动速度

    bool is_day = true;
    bool is_day2 = true;
    bool is_day3 = true;

    int A = 0;//背景颜色,切换过程慢慢增大

    //关于上层白云为什么不像下层白云一样整合成一个绘制路径，
    //原因：考虑后期为每一朵云添加随机飘动功能（上下左右的小幅震动）

    QPainterPath under_cloude_path;//下层白云的绘制路径
    int under_cloude_path_dy = 0;//下层白云向下移动的距离

    QPainterPath star_path;//星星的绘制路径
    int star_dy = 0;//星星向下移动的距离

public:
    double getmove_speed();
    void setmove_speed(int time = 600);
    bool Is_day();

};

#endif // DAY_TO_NIGHT_FRAME_H
