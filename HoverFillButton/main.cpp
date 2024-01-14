#include <QApplication>
#include <QWidget>

#include <QRadialGradient>
#include "hoverfillbutton.h"

class Ui
{
public:
    HoverFillButton *pushButton_1;
    HoverFillButton *pushButton_2;
    HoverFillButton *pushButton_3;
    HoverFillButton *pushButton_4;
    HoverFillButton *pushButton_5;
    HoverFillButton *pushButton_6;

    void setupUi(QWidget *parent)
    {
        parent->resize(420, 314);
        pushButton_1 = new HoverFillButton(parent);
        pushButton_1->setGeometry(QRect(70, 90, 130, 50));
        pushButton_2 = new HoverFillButton(parent);
        pushButton_2->setGeometry(QRect(210, 90, 130, 50));
        pushButton_3 = new HoverFillButton(parent);
        pushButton_3->setGeometry(QRect(70, 150, 130, 50));
        pushButton_4 = new HoverFillButton(parent);
        pushButton_4->setGeometry(QRect(210, 150, 130, 50));
        pushButton_5 = new HoverFillButton(parent);
        pushButton_5->setGeometry(QRect(70, 210, 130, 50));
        pushButton_6 = new HoverFillButton(parent);
        pushButton_6->setGeometry(QRect(210, 210, 130, 50));

        pushButton_1->setText("CircularFill");
        pushButton_2->setText("CrossFill");
        pushButton_3->setText("DiagonalRectangle");
        pushButton_4->setText("BottomCircle");
        pushButton_5->setText("LeftRectangle");
        pushButton_6->setText("CircularFill");
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    w.setWindowTitle("HoverFillButton");
    w.setObjectName("qw");
    w.setStyleSheet("#qw{background-color:rgb(125,125,125);}");

    Ui ui;
    ui.setupUi(&w);


    for (auto &btn : w.findChildren<HoverFillButton *>())
        btn->setFillSpeed(4); // 设置填充速度,越小越快,最小为1,不同的动画类型可以考虑使用不同的速度,以达到最佳观感

    // 配合样式表使用示例
    // 配合样式表,添加阴影,形成类似发光效果
    ui.pushButton_1->setFillBrush(QColor(255, 124, 118));//设置填充颜色
    ui.pushButton_1->setStyleSheet("background-color:rgba(240,152,152,50);"
                                   "border-radius:10");
    ui.pushButton_1->refreshRadius(10); // 使用样式表设置border-radius后应调用refreshRadius,同步更新绘制圆角
    ui.pushButton_1->addShadow(0, 0, 100, QColor(255, 150, 150)); // 添加阴影

    // 设置动画类型
    // 默认值//ui.pushButton_1->setAnimationType(HoverFillButton::AnimationType::CircularFill);//进入点圆形填充
    ui.pushButton_2->setAnimationType(HoverFillButton::AnimationType::CrossFill);         // 两侧圆形填充
    ui.pushButton_3->setAnimationType(HoverFillButton::AnimationType::DiagonalRectangle); // 斜着的矩形填充
    ui.pushButton_4->setAnimationType(HoverFillButton::AnimationType::BottomCircle);      // 底部圆形填充
    ui.pushButton_5->setAnimationType(HoverFillButton::AnimationType::LeftRectangle);     // 左侧矩形填充


    // 设置文字颜色示例
    ui.pushButton_2->setTextColor(Qt::red, Qt::green); // 普通状态下的颜色,悬浮状态下的颜色

    // 设置自定义填充画刷示例
    // 绘制桃红色和淡绿色的渐变背景
    QPointF point(ui.pushButton_2->width(),ui.pushButton_2->height());//渐变中心点
    QRadialGradient gradient(point, ui.pushButton_2->width()/1.3, point);//QRadialGradient(圆形渐变区域中心，渐变区域半径，聚焦点坐标)
    gradient.setColorAt(0.5, QColor(170, 255, 127));
    gradient.setColorAt(1.0, QColor(255, 150, 150));
    ui.pushButton_2->setFillBrush(QBrush(gradient));//设置渐变画刷

    w.show();
    return a.exec();
}
