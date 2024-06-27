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

    void setLinePen( const QPen &focusInPen,
                    const QPen &focusOutPen = QPen(QBrush(QColor(66,66,66)),2)); // 设置线条的绘制画笔，参数：获取焦点的绘制画笔，失去焦点的绘制画笔
    void setTextColor(const QColor &textColor);                                 // 设置输入文字颜色
    void setPlaceholderTextColor(const QColor &placeholderText);                // 设置预设背景文字颜色
    void setLineSpeed(int speed);                                               // 设置线条填速度,越小越快，最小为1,实际使用可根据文本框的长度不同设置不同的速度以达到最佳观感

private:
    int right_coordinate{-1};   // 线条右侧坐标
    QTimer *timer;
    QPen inactive_pen;// 未获得焦点时线条的绘制画笔
    QPen active_pen; // 获得焦点时线条的绘制画笔

    void initializeMemberVariable();

private slots:
    void inFocus();  // 获得焦点
    void outFocus(); // 失去焦点

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;  // 获取焦点事件
    virtual void focusOutEvent(QFocusEvent *event) override; // 失去焦点事件
};

#endif // DynamicUnderlineLineEdit_H
