/*
改写自PyQt-Fluent-Widgets组件库
请遵循 GPL-3.0 开源协议

原作者：bilibili@呆唯男友，uid：471587058
PyQt-Fluent-Widgets链接：https://github.com/zhiyiYo/PyQt-Fluent-Widgets
源代码语言：Python
*/


#include <QApplication>
#include <QWidget>

#include <QPushButton>
#include "flowlayout.h"
class Widget:public QWidget
{
public:
    Widget(QWidget* parent = nullptr):
        QWidget::QWidget{parent}
    {
        layout = new FlowLayout(this, true);

        //customize animation
        layout->setAnimation(250, QEasingCurve::OutQuad);
        layout->setContentsMargins(30, 30, 30, 30);
        layout->setVerticalSpacing(20);
        layout->setHorizontalSpacing(10);

        QPushButton* btn1 = new QPushButton("aiko");
        QPushButton* btn2 = new QPushButton("刘静爱");
        QPushButton* btn3 = new QPushButton("柳井爱子");
        QPushButton* btn4 = new QPushButton("aiko 赛高");
        QPushButton* btn5 = new QPushButton("aiko 太爱啦😘");

        layout->addWidget(btn1);
        layout->addWidget(btn2);
        layout->addWidget(btn3);
        layout->addWidget(btn4);
        layout->addWidget(btn5);

        resize(250,300);
        setObjectName("Widget");
        setStyleSheet(R"(#Widget{
                            background: white;}
                         QPushButton{
                            padding: 5px 10px;
                            font:15px "Microsoft YaHei";
                         })");
    }
private:
    FlowLayout* layout;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
