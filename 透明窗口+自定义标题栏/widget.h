#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QPainterPath>
#include <QMouseEvent>
#include <QPointF>
#include <QPushButton>
#include <QFrame>
#include <QCheckBox>
#include <QResizeEvent>
#include <Windows.h>
#include <QSize>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    virtual void paintEvent(QPaintEvent *event) override;           //绘制事件（绘制透明背景）
    virtual void mousePressEvent(QMouseEvent *event) override;      //鼠标按下事件（记录坐标等参数）
    virtual void mouseMoveEvent(QMouseEvent *event) override;       //鼠标移动（窗口拖动，窗口大小改变，鼠标图标改变）
    virtual void resizeEvent(QResizeEvent *event) override;         //窗口大小改变事件（移动标题栏控件）
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;//鼠标双击事件（全屏和恢复大小）


private:
    enum class direction : int// 声明类内枚举 方向:无,上,下,左,右,左上,左下,右上,右下
    {
        none,
        top, bottom, left, right,
        left_top, left_bottom, right_top,right_bottom
    };

private:
    Ui::Widget *ui;

    QPointF m_click;        //鼠标点击坐标
    QPointF w_left_top;     //窗口左上角坐标
    QPointF w_left_bottom;  //窗口左下角坐标
    QPointF w_right_top;    //窗口右上角坐标

    QFrame* line;           //水平分割线
    QPushButton* exitbtn;   //关闭按钮
    QPushButton* maxsize;   //最大化按钮
    QPushButton* minisize;  //最小化按钮

    QSize min_W_size;  //窗口最小尺寸

    //背景颜色RGBA值
    int bkR = 0;
    int bkG = 0;
    int bkB = 0;
    int bkA = 100;//基础透明度

    int boundary_width = 15; //边界区域的宽度
    int border_radius = 15; //窗口圆角半径

    bool isresize = false;//是否在调整大小
    bool isdirectionLock = false;//改变大小的方向是否锁定
    bool ismove = false;//是否在移动窗口

    direction resize_direction = direction::none;//默认调整方向

};

#endif // WIDGET_H
