#include <QApplication>

#include "hoverfillbutton.h"
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;

    HoverFillButton *btn1 = new HoverFillButton(&w);
    btn1->move(290, 180);
    btn1->setText("PushButton1", QColor(Qt::black), QFont("微软雅黑", 10));
    btn1->resize(130, 50);
    btn1->SetFillSpeed(5);
    btn1->addShadow(0,0,100,Qt::red);//添加阴影

    HoverFillButton *btn2 = new HoverFillButton(&w);
    btn2->move(430, 180);
    btn2->setText("Pushbutton2", QColor(Qt::black), QFont("微软雅黑", 10));
    btn2->resize(130, 50);
    btn2->SetFillSpeed(5);
    btn2->setTextBehind(true); // 设置文字在后面

    QLabel *t1 = new QLabel("0", &w);
    t1->resize(50, 20);
    t1->move(355, 250);
    t1->setFont(QFont("微软雅黑", 10));
    QObject::connect(btn1, &HoverFillButton::clicked, &w, [=]
                     { t1->setText(QString::number(t1->text().toInt() + 1)); });

    QLabel *t2 = new QLabel("0", &w);
    t2->resize(50, 20);
    t2->move(495, 250);
    t2->setFont(QFont("微软雅黑", 10));
    QObject::connect(btn2, &QPushButton::clicked, &w, [=]
                     { t2->setText(QString::number(t2->text().toInt() + 1)); });

    w.resize(640, 480);
    w.show();
    return a.exec();
}
