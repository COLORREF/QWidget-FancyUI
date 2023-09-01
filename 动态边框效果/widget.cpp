#include "widget.h"
#include "ui_widget.h"
#include <QLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    for(auto lay : findChildren<QLayout*>())
    {
        lay->setContentsMargins(2,2,2,2);//设置边距即流光粗细
    }
    QPixmap pix(":/paimeng.png");
    ui->label->setPixmap(pix);
    ui->label->setScaledContents(true);//缩放图片，填充所有空间

    auto func = [=]()->QColor
    {
        return QColor(UniformDist_int(0,255,1).at(0),
                      UniformDist_int(0,255,1).at(0),
                      UniformDist_int(0,255,1).at(0), 255);
    };

    ui->frame->setDynamicColor(func(),func());
    ui->frame_2->setDynamicColor(func(),func());
    ui->frame_3->setDynamicColor(func(),func());
    ui->frame_4->setDynamicColor(func(),func());
    ui->frame_5->setDynamicColor(func(),func());
    ui->frame_6->setDynamicColor(func(),func());
    ui->frame_7->setDynamicColor(func(),func());
    ui->frame_8->setDynamicColor(func(),func());
    ui->frame_9->setDynamicColor(func(),func());

    //别问为什么不用findChildren
    //用findChildren会崩，我也不知道为什么？O.o😣
}



Widget::~Widget()
{
    delete ui;
}

QList<int> Widget::UniformDist_int(const int minRange, const int maxRange, const int quantity)
{
    assert(quantity >= 1);
    if (quantity < 1)
    {
        throw QException();
    }

    QList<int> RandNum;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniformDist(minRange, maxRange);

    for (int i = 0; i < quantity; i++)
    {
        RandNum.push_back(uniformDist(gen)); // 生成随机整数
    }
    return RandNum;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.fillRect(event->rect(), qRgb(182,189,193));
}
