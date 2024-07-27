#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QObject>
#include <QWidget>
#include <QString>
#include "titlebar.h"

//如果需要自定义一些按钮，可以继承此类
class ToolButton : public TitleBarButton
{
    Q_OBJECT
public:
    enum class ToolType
    {
        CUSTOM,//自定义，此枚举值将不会设置默认的图标
        GEAR,//齿轮
        WRENCH,//扳手
        QUESTION,//问号
        SEARCH,//搜索
        WIFI,//wifi
        BLUETOOTH,//蓝牙
        THREE_HORIZONTAL_LINES,//三条水平线
        THREE_HORIZONTAL_POINTS,//三个水平点
        SUN_MOON,//太阳和月亮
        LEFT_ARROW,//左箭头
    };
    explicit ToolButton(QWidget* parent, ToolButton::ToolType type = ToolType::CUSTOM);

    //setIcon会使用固定的图标，如果希望深浅主题改变时更换对应的图标，调用此函数设置对应图标即可
    //但初始的图标仍需调用setIcon来显示，否则初始显示将会没有图标
    void setThemeIcon(const QPixmap& light, const QPixmap& dark);
    void setFixedSize(const QSize& size);
    void setFixedSize(int w,int h);
    void setFixedHeight(int h);
    void setFixedWidth(int w);
    void resizeIconSize(const QSize& size);
    void cancelsFixedSize();//取消固定大小属性，使之能够响应布局，在调用resize之前也需要调用此函数
protected slots:
    virtual void onThemeChange(Theme::Type themeType)override;

private:
    QPixmap light;
    QPixmap dark;
    ToolType type = ToolType::CUSTOM;
};

#endif // TOOLBUTTON_H
