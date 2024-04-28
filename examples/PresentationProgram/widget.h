#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QWidget>
#include "../../RetractableSidebarWindow/sidebarwindow.h"
#include "../../RetractableSidebarWindow/sidebaroptionsbutton.h"

class Ui_HoverFillButton;
class Ui_DynamicUnderlineLineEdit;
class Ui_Set;

class Widget : public SidebarWindow
{
    Q_OBJECT
public:
    Widget(QWidget* parent=nullptr);
    ~Widget();

private:
    void setupSidebar(Sidebar *parent);
    void setupStackedWidget(QStackedWidget * parent);
    void connectSignals();

    Ui_HoverFillButton* ui_HoverFillButton;
    Ui_DynamicUnderlineLineEdit* ui_DynamicUnderlineLineEdit;
    Ui_Set* ui_Set;

    QWidget* page_HoverFillButton;//第一页 page_HoverFillButton
    QWidget* page_DynamicUnderlineLineEdit;//第二页 page_DynamicUnderlineLineEdit
    QWidget* page_Set;//设置页

    SidebarOptionsButton *btn_page_HoverFillButton;//第一页按钮
    SidebarOptionsButton * btn_page_DynamicUnderlineLineEdit;//第二页按钮
    QSpacerItem *verticalSpacer;//垂直弹簧
    SidebarOptionsButton *btn_page_Set;//设置页按钮
    QPushButton *btn_expand;//展开按钮
};

#endif // WIDGET_H
