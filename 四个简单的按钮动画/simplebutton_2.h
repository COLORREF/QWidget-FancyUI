#ifndef SIMPLEBUTTON_2_H
#define SIMPLEBUTTON_2_H

#include <QFont>
#include <QtMath>
#include <QTimer>
#include <QColor>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPalette>
#include <QShowEvent>
#include <functional>
#include <QPaintEvent>
#include <QPushButton>
#include <QEnterEvent>
#include <QPainterPath>

using std::function;

class SimpleButton_2 : public QPushButton
{
    Q_OBJECT
private:
    QColor fill_color = qRgb(0,169,127);            //填充颜色
    QColor font_initial_color = qRgb(0,169,127);    //字体初始颜色
    QColor font_hover_color = Qt::white;            //悬停时字体颜色
    QColor font_paint_color = font_initial_color;   //绘制时字体颜色颜色
    QPainterPath path;                              //绘制路径
    QTimer* timer;
    QString text;                                   //按钮文字

    int border_radius = 10;    //圆角半径
    int width_variation = 3;   //宽度改变量
    int initial_width = 0;     //起始宽度
    int max_width;             //最大宽度
    int fill_speed = 5;        //填充速度,越大越慢

    function<void()> WidthIncrease{
        [&]{
            initial_width += width_variation;
            if(initial_width > max_width)
                timer->stop();
            update();
        }
    };

    function<void()> WidthDecrease{
        [&]{
            initial_width -= width_variation;
            if(initial_width < 0)
            {
                initial_width = 0;
                timer->stop();
            }
            update();
        }
    };

public:
    SimpleButton_2(QWidget* parent);

    void setfillSpeed(const int speed);

    void setFillColor(const QColor& fillcolor);

    void setFontColor(const QColor& initial, const QColor& hover);

    void setText(const QString& text);

    void setBackgroundColor(const QColor& bk_color);                                    //设置输入框背景颜色

    void setBorderRadius(const int border_radius);                                      //设置输入框圆角半径

    void setBorderColor(const QColor& border_color, const int border_thickness = 1);    //设置边框颜色

    void setStyleSheet(const QString& stylesheet);                                      //重写设置样式表函数

protected:
    virtual void paintEvent(QPaintEvent* event) override;

    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void enterEvent(QEnterEvent* event) override;

    virtual void leaveEvent(QEvent* event) override;

};

#endif // SIMPLEBUTTON_2_H
