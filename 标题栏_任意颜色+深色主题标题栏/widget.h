#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {class Widget;}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void changeTitleBarColor();

    // 设置或取消标题栏深色模式
    bool SetTitleBarDarkMode(bool enableDarkMode);

private:
    Ui::Widget *ui;

    //设置标题栏颜色(不支持透明度)
    bool SetTitleBarColor(int red, int green, int blue);
};
#endif // WIDGET_H
