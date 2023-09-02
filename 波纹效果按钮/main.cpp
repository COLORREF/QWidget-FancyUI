#include <QApplication>
#include "mypushbutton_QFrame.h"
#include "mypushbutton_QPushButton.h"
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;

    mypushbutton_QPushButton* btn1 = new mypushbutton_QPushButton(&w);
    btn1->move(290, 180);
    btn1->setText("PushButton1", QFont("微软雅黑",10), QColor(Qt::black));
    btn1->resize(130, 50);
    btn1->setTimeInterval(5);

    mypushbutton_QFrame* btn2 = new mypushbutton_QFrame(&w);
    btn2->move(430, 180);
    btn2->setText("Pushbutton2", QFont("微软雅黑",10), QColor(Qt::black));
    btn2->resize(130, 50);
    btn2->setTimeInterval(5);

    //Win11按钮默认样式
    btn2->setStyleSheet("*{"
                      "border: none; "
                      "border-radius: 5px;}"

                      "QPushButton{"
                      "background-color: rgba(0, 0, 0, 0);}"    //一定要把按钮背景色设置成透明才能显现效果，因为本质实在QFrame上绘制

                      "QFrame{"
                      "background-color: rgb(253,253,253);"
                      "border:1px solid rgb(208,208,208);"
                      "border-bottom: 1px solid rgb(186,186,186);}"

                      "QFrame:hover{"
                      "background-color: rgb(224,238,249);"
                      "border:1px solid rgb(0,120,212);"
                      "border-bottom: 1px solid rgb(0,107,190);}"
                       );

    QLabel* t1 = new QLabel("0",&w);
    t1->resize(50,20);
    t1->move(355,250);
    t1->setFont(QFont("微软雅黑",10));
    QObject::connect(btn1, &mypushbutton_QPushButton::clicked, &w, [=]{t1->setText(QString::number(t1->text().toInt()+1));});

    QLabel* t2 = new QLabel("0",&w);
    t2->resize(50,20);
    t2->move(495,250);
    t2->setFont(QFont("微软雅黑",10));
    QObject::connect(btn2->btn, &QPushButton::clicked, &w, [=]{t2->setText(QString::number(t2->text().toInt()+1));});

    w.resize(640,480);
    w.show();
    return a.exec();
}












































