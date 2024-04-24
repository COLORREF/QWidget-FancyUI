#ifndef CUSTOMBUTTONBASE_H
#define CUSTOMBUTTONBASE_H

#include <QPushButton>
#include <QWidget>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QColor>

class CustomButtonBase : public QPushButton
{
public:
    explicit CustomButtonBase(QWidget *parent = nullptr);
    explicit CustomButtonBase(const QString &text, QWidget *parent = nullptr);
    CustomButtonBase(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    void addShadow(int right, int down, qreal blurRadius, const QColor &shadowColor = Qt::black); // 添加阴影效果,参数:右偏移量,下偏移量,模糊半径,阴影颜色
    void setTextColor(const QColor& normalTextColor, const QColor &hoverTextColor);                // 设置文字未悬浮时颜色和悬浮时颜色
    QPalette &palette() const;                                                                    // 获取调色板的引用(返回非const引用)
    void refreshRadius(int radius);                                                               // 使用样式表修改修改圆角半径后，应调用此函数，同步更新填充绘制的圆角半径

protected:
    QColor hover_text_color{Qt::black}; //  悬停时文字颜色
    QPainterPath path;                  // 绘制路径
    bool is_cursor_inside = false;      // 光标是否在内部
    int radius = 4;                     // 圆角半径

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    void initializeMemberVariable();
};

#endif // CUSTOMBUTTONBASE_H
