#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "FancyWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE


class AnimationIndicatorSidebar;

class SidebarWidget : public AdaptiveLayoutWindow
{
    Q_OBJECT

public:
    SidebarWidget(QWidget *parent);
    ~SidebarWidget();
protected:
    AnimationIndicatorSidebar* _aniSidebar;
};


class Widget : public SidebarWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    void showControl();
    void showSidebar();
    void showImageProcessing();
    void showIcons();
private slots:
    void setThemeColor();
};
#endif // WIDGET_H
