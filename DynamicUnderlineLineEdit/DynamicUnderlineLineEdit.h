#ifndef DynamicUnderlineLineEdit_H
#define DynamicUnderlineLineEdit_H

#include <QWidget>
#include <QLineEdit>
#include <QPainter>
#include <QPaintEvent>
#include <QFocusEvent>
#include <QTimer>
#include <QColor>
#include <QPointF>
#include <QPalette>

//动态下划线单行文本框：Dynamic Underline LineEdit
class DynamicUnderlineLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit DynamicUnderlineLineEdit(QWidget *parent = nullptr);
    explicit DynamicUnderlineLineEdit(const QString &text, QWidget *parent = nullptr);

    void setLineColor(const QColor &focusOutcolor, const QColor &focusIncolor); // 设置线条颜色，参数：失去焦点颜色，获取焦点颜色
    void setFontColor(const QColor &fontColor);                                 // 设置输入文字颜色
    void setPlaceholderTextColor(const QColor &placeholderText);                // 设置预设背景文字颜色
    void setLineSpeed(int speed);                                               // 设置线条填速度
    void setLineHeight(int lineHeight);                                         // 设置线条像素高度

private:
    int line_height{2};        // 线的高度(像素)
    int line_width_anim{-1};   // 线条长度(坐标)
    int line_width_offset{10}; // 线条长度的增量
    int msec{10};              // 定时器间隔
    QTimer *timer;
    QColor line_color_1{qRgb(66, 66, 66)};  // 未获得焦点时的颜色
    QColor line_color_2{qRgb(0, 123, 255)}; // 获得焦点时的颜色

private slots:
    void inFocus();  // 获得焦点
    void outFocus(); // 失去焦点

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;  // 获取焦点事件
    virtual void focusOutEvent(QFocusEvent *event) override; // 失去焦点事件
};

#endif // DynamicUnderlineLineEdit_H
