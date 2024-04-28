#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QObject>
#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSize>
#include <QPainter>
#include <QPen>
#include <QVBoxLayout>
#include <QBrush>
#include <QColor>

/*
 * Sidebar默认具有垂直布局
 * sidebarOptionsButton的位置确定依赖于Sidebar默认的垂直布局
 * childrenCumulativeHeight函数计算子控件累计高度,也依赖于Sidebar默认的垂直布局
 * 改变Sidebar布局可能会使子控件位置错误
 * 若不清楚位置确定的逻辑,不建议修改任何有关Sidebar布局的参数
 */
class Sidebar : public QWidget
{
    Q_OBJECT
public:
    explicit Sidebar(QWidget *parent, int initialWidth = 50);
    ~Sidebar();

    void addWidget(QWidget *btn);                   // 添加控件
    void addItem(QLayoutItem *item);                // 添加弹簧
    void setIncreasedWidth(quint32 increasedWidth); // 设置展开增加宽度
    void setExpandTime(int ms);                     // 设置展开时间
    void setBackgroundBrush(const QBrush &brush);   // 设置背景色绘制笔刷
    void setBorderLinePen(const QPen &pen);         // 设置右侧边界线绘制笔
    int childrenCumulativeHeight();                 // 计算子控件的累计总高度，包括布局边距和控件间距

public slots:
    void autoExpand(); // 自动控制展开或收缩
    void expand();     // 展开
    void shrink();     // 收缩

protected:
    void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent *) override;

signals:
    void expandStart();    // 开始展开
    void expandFinished(); // 展开结束
    void shrinkStart();    // 开始收缩
    void shrinkFinished(); // 收缩结束
    void finished();       // 展开或收缩结束

private:
    bool isAnimationRunning = false;             // 动画是否正在运行
    bool isExpanded = false;                     // 是否展开
    QPropertyAnimation *animation = nullptr;     // 属性动画
    QVBoxLayout *verticalLayout = nullptr;       // 垂直布局
    QSize initialSize;                           // 初始尺寸
    QSize endSize;                               // 终止尺寸
    QBrush backgroundBrush{qRgb(243, 243, 243)}; // 背景色绘制笔刷
    QPen borderLinePen{qRgb(229, 229, 229)};     // 右侧边界线绘制笔
};

#endif // SIDEBAR_H
