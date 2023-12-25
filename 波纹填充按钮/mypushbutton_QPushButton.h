/*************************
 * Qt版本：Qt 6.2.4
 * qmake构建
 * 完成时间：2023/8/31
 * 最后一次更新时间：2023/12/25
 * 基类：QPushButton
 ****************************************
 * 灵感来源：B站：Rev_RoastDuck UID：439475163
 ********************************************************
 * 作者：B站：EMC方 UID：1843315943  
 * 项目地址：https://github.com/BFEMCC/Qt-widget-Fancy_UI
 ****************************************************
 * 在Python代码等效的C++代码上，进行了进一步优化和修改
 * 进行了接口拓展，使用更方便
 * 修改了构造函数，支持在设计界面中直接使用（提升为...）
 ************************************************************
 * 请确保Qt大版本为 Qt6
 * 如使用Qt5，可尝试将enterEvent(QEnterEvent* event)函数的参数类型改为QEvent*
 * 并参考注释和leaveEvent函数修改其余函数内容，以防止报错
 * 可能还会有其他错误，请自行查阅资料修改
 ******************************************************************************/

#ifndef MYPUSHBUTTON_QPUSHBUTTON_H
#define MYPUSHBUTTON_QPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QFont>
#include <QTimer>
#include <QEnterEvent>
#include <QEvent>
#include <QCursor>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPen>
#include <QRgb>
#include <QPainterPath>
#include <QRegularExpression>
#include <QtMath>
#include <QPalette>
#include <QPointF>

class mypushbutton_QPushButton : public QPushButton
{
private:
    int timeInterval = 10;       // 定时器时间间隔，单位：ms
    QTimer *timer = Q_NULLPTR;   // 定时器对象
    QPointF mouse_point;         // 记录鼠标进入和离开时的坐标
    int max_radius;              // 最大半径
    int radius = 0;              // 绘制半径
    int radius_var = 2;          // 半径每次改变的值（增大或减小）
    QColor fill_color;           // 填充颜色
    int btn_radius = 4;          // 填充色块的圆角半径
    bool is_text_behind = false; // 文本是否在填充颜色后面
    QPainterPath path;           // 绘制路径

public:
    mypushbutton_QPushButton(QWidget *parent = Q_NULLPTR);

    mypushbutton_QPushButton(const QString &text, QWidget *parent = nullptr);

    void SetFillSpeed(const int fillSpeed); // 设置填充速度（设置定时器时间间隔，控制填充速度）

    void setText(const QString &text, const QColor &fontcolor = Qt::black, const QFont &font = QFont("微软雅黑")); // 设置文本内容

    void setFillColor(const QColor &fillcolor); // 设置填充颜色

    void setTextBehind(bool is_text_behind = false); // 设置文本是否在填充颜色后面

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    virtual void enterEvent(QEnterEvent *event) override;

    virtual void leaveEvent(QEvent *event) override;

    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // MYPUSHBUTTON_QPUSHBUTTON_H
