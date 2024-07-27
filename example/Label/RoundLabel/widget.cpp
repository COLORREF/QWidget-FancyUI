#include "widget.h"
#include "Label/RoundLabel/roundlabel.h"
#include "utils/imageutils.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);
    label = new RoundLabel(this);
    label_2 = new RoundLabel(this);
    gridLayout->addWidget(label, 0, 0, 1, 1);
    gridLayout->addWidget(label_2,0,1,1,1);

    // FImage是对QImage的封装，可以无缝转换QImage、QPixmap
    FImage fimage(":/star_rail.png");
    label->setPixmap(fimage.toQPixmap());
    label_2->setPixmap(fimage.GaussianBlur(48).toQPixmap());//对图片进行高斯模糊

    this->resize(400, 400);
}
