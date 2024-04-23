#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include "sidebarwindow.h"
#include "sidebaroptionsbutton.h"

class Ui_page;

class Widget : public SidebarWindow
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void setupSidebar(Sidebar *parent);
    void setupStackedWidget(QStackedWidget *parent);

    Ui_page *page_ui;

    QWidget *page_1;   // 第一页
    QWidget *page_2;   // 第二页
    QWidget *page_3;   // 第三页
    QWidget *page_set; // 设置页

    SidebarOptionsButton *btn_page_1; // 第一页按钮
    SidebarOptionsButton *btn_page_2; // 第二页按钮
    SidebarOptionsButton *btn_page_3; // 第三页按钮
    QSpacerItem *verticalSpacer;      // 垂直弹簧
    SidebarOptionsButton *btn_set;    // 设置按钮
    SidebarOptionsButton *btn_expand; // 展开按钮
};

#endif // WIDGET_H
