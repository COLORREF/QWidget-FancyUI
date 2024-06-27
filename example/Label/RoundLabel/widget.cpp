#include "widget.h"
#include "RoundLabel/roundlabel.h"
#include "image_utils/imageutils.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);
    label = new RoundLabel(this);
    gridLayout->addWidget(label, 0, 0, 1, 1);
    label->setPixmap(ImageUtils::GaussianBlur(QPixmap(":/star_rail.png"),48));//对图片进行高斯模糊
    this->resize(400, 400);
}
