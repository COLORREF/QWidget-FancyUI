/*************************
 * Qt版本：Qt 6.4.2
 * 使用qmake构建项目
 * 完成时间：2023/8/30
 * 更新时间：2023/8/31
 * 基类：QFrame
 ****************************************
 * **用C++重写Python的代码，并优化和修改
 * **Python代码作者：B站：Rev_RoastDuck UID：439475163
 * **C++代码作者：B站：EMC方 UID：1843315943
 * **C++版本 GitHub：https://github.com/BFEMCC/Qt-widget-Fancy_UI
 ********************************************************
 * ***在Python代码等效的C++代码上，进行了进一步优化，
 * ***Release编译后，相比Python版，cpu占用略微降低，内存占用降低了大概5倍！，极大的优化了性能（不同电脑环境可能不一样）
 * ***这得益于C++语言本身，以及少量代码方面的优化
 * ***C++代码在原先的基础上进行了一些修改和接口拓展，使用更方便
 * ***修改了构造函数，支持在设计界面中直接使用（提升为...）
 ************************************************************
 * ****请确保Qt大版本为 Qt6
 * ****如使用Qt5，可尝试将enterEvent(QEnterEvent* event)函数的参数类型改为QEvent*，
 * ****并参考注释和leaveEvent函数修改函数内容，以防止报错
 * ****可能还会有其他错误，请自行修改
******************************************************************************/

#ifndef mypushbutton_QFrame_QFRAME_H
#define mypushbutton_QFrame_QFRAME_H
#include <QWidget>
#include <QFrame>
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
#include <QtMath>
#include <QPointF>
#include <QRegularExpression>

class mypushbutton_QFrame : public QFrame
{
private:
    int timeInterval = 10;              //定时器时间间隔，单位：ms
    QTimer* timer = Q_NULLPTR;          //定时器对象
    QPointF mouse_point;                //记录鼠标进入和离开时的坐标
    int max_radius;                     //最大半径
    int radius = 0;                     //绘制半径
    int radius_var = 2;                 //半径每次改变的值（增大或减小）
    QColor fill_color;                  //填充颜色
    int btn_radius = 0;                 //按钮圆角半径

public:
    QPushButton* btn = Q_NULLPTR;       //按钮对象

public:
    mypushbutton_QFrame(QWidget* parent = Q_NULLPTR);           //构造函数

    void setTimeInterval(const int timeinitval);                //设置定时器时间间隔，控制填充速度

    void setText(const QString& text,
                 const QFont& font = QFont("微软雅黑"),
                 const QColor& fontcolor = Qt::white);          //设置文本内容

    void setFillColor(const QColor& fillcolor);                 //设置填充颜色

    void setStyleSheet(const QString& style);                   //重写父类setStyleSheet（“重写”这个词在这里可能用的不恰当）

protected:
    virtual void enterEvent(QEnterEvent* event) override;       //重写鼠标进入事件

    virtual void leaveEvent(QEvent*) override;                  //重写鼠标离开事件

    virtual void paintEvent(QPaintEvent* event) override;       //重写绘图事件

    virtual void resizeEvent(QResizeEvent *event) override;     //重写改变大小事件

};

#endif // mypushbutton_QFrame_QFRAME_H
