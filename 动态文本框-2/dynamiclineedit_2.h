#ifndef DYNAMICLINEEDIT_2_H
#define DYNAMICLINEEDIT_2_H

#include <QFont>
#include <QRect>
#include <QSize>
#include <QLabel>
#include <QEvent>
#include <QColor>
#include <QPoint>
#include <QFrame>
#include <QString>
#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPropertyAnimation>

class DynamicLineEdit_2 : public QFrame
{
    Q_OBJECT

private:
    QLabel* TipsLabel = nullptr;                                                //提示文字标签
    QLineEdit* edit = nullptr;                                                  //单行输入框

    QPropertyAnimation* Edit_Animation = nullptr;                               //输入框动画
    QPropertyAnimation* TipsLabel_Animation = nullptr;                          //提示标签动画

    int Edit_Initial_H = 2;                                                     //初始高度
    int Edit_Initial_y;                                                         //初始位置y坐标
    int Edit_Expansion_H = 35;                                                  //展开时高度
    int Edit_Expansion_y;                                                       //展开时y坐标

    int TipsLabel_H;                                                            //初始高度
    int TipsLabel_Initial_x = 10;                                               //初始位置x坐标
    int TipsLabel_Initial_y;                                                    //初始位置y坐标

public:
    DynamicLineEdit_2(QWidget* parent);

    void setEditExpansionHeight(int edit_expansion_height);                     //设置输入框展开后的高度

    void setTipsText(const QString& text);                                      //设置提示文本内容

    void setEditFontColor(const QColor& fontColor);                             //设置输入框的文字颜色

    void setEditFont(const QFont& font, const QColor& fontcolor = Qt::black);   // 设置输入框的字体和文字颜色

    void setTipsFontColor(const QColor& fontColor);                             //设置提示文本的文字颜色

    void setTipsFont(const QFont& font, const QColor& fontcolor = Qt::black);   //设置提示文本的字体和文字颜色

    void setEditBackgroundColor(const QColor& bk_color);                        //设置输入框背景颜色

    void setBorderRadius(const int border_radius = 5);                          //设置输入框圆角半径

    void setBorderColor(QColor border_color, int border_thickness = 1);         //设置边框颜色

    void setStyleSheet(const QString& stylesheet);                              //重写设置样式表函数

    void setUpspringTime(int time = 200);                                       //设置文本框弹起时间

    QString text();                                                             //获取输入框文本内容

    bool hasFocus();                                                            //获取输入框焦点状态

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;                  //鼠标按下事件

    virtual void leaveEvent(QEvent* event) override;                            //鼠标离开事件

    virtual void resizeEvent(QResizeEvent* event) override;                     //改变大小事件

private:
    void setPosParameters();                                                    //设置控件坐标等参数

    void setAnimationProperties();                                              //设置动画属性
};

#endif // DYNAMICLINEEDIT_2_H
