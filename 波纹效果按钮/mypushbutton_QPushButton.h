/*************************
 * Qt版本：Qt 6.4.2
 * 使用qmake构建项目
 * 完成时间：2023/8/31
 * 更新时间：2023/8/31
 * 基类：QPushButton
 * 注释请参考 mypushbutton_QFrame.h
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
#include <QPointF>

class mypushbutton_QPushButton : public QPushButton
{
private:
    int timeInterval = 10;
    QTimer* timer = Q_NULLPTR;
    QPointF mouse_point;
    int max_radius;
    int radius = 0;
    int radius_var = 2;
    QColor fill_color;
    int btn_radius = 4;

public:
    mypushbutton_QPushButton(QWidget* parent = Q_NULLPTR);

    void setTimeInterval(const int timeinitval);

    void setText(const QString& text, const QFont& font = QFont("微软雅黑"), const QColor& fontcolor = Qt::black);

    void setFillColor(const QColor& fillcolor);

    void setStyleSheet(const QString& style);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

    virtual void enterEvent(QEnterEvent* event) override;

    virtual void leaveEvent(QEvent* event) override;

    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // MYPUSHBUTTON_QPUSHBUTTON_H
