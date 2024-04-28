#ifndef SIDEBARWINDOW_H
#define SIDEBARWINDOW_H

#include <QWidget>
#include <QResizeEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QStackedWidget>
#include "sidebar.h"

class SidebarWindow : public QWidget
{
    Q_OBJECT

public:
    SidebarWindow(QWidget *parent = nullptr);
    Sidebar *sidebar();               // 返回侧边栏对象指针
    QSpacerItem *placeholderSpring(); // 返回占位弹簧对象指针
    QStackedWidget *stackedWidget();  // 返回多页窗口控件对象指针

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override; // 点击

signals:
    void clicked(); // 多页窗口被点击时此信号将发出

private:
    void setupUi(QWidget *parent);
    Sidebar *sidebar_;               // 侧边栏
    QHBoxLayout *horizontalLayout;   // 水平布局
    QSpacerItem *placeholderSpring_; // 占位弹簧
    QStackedWidget *stackedWidget_;  // 多页窗口
};
#endif // SIDEBARWINDOW_H
