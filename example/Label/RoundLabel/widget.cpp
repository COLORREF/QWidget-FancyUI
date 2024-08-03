#include "widget.h"
#include "Label/RoundLabel/roundlabel.h"
#include "utils/imageutils.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout* gridLayout = new QGridLayout(this);

    QList<RoundLabel*> labels;
    QList<FImage> fimages;
    for(int row = 0,i = 0; row < 3; row++)
    {
        for(int column = 0; column < 3; column++,i++)
        {
            labels.append(new RoundLabel(this));
            gridLayout->addWidget(labels[i],row,column,1,1);
            fimages.append(FImage(":/star_rail.png"));
        }
    }

    // FImage是对QImage的封装，可以无缝转换QImage、QPixmap
    // FImage的成员函数会修改原始图片

    labels[0]->setPixmap(fimages[0].toQPixmap());//原图
    labels[1]->setPixmap(fimages[1].gaussianBlur(96).toQPixmap());//高斯模糊
    labels[2]->setPixmap(fimages[2].horizontalGaussianBlur(96).toQPixmap());//水平高斯模糊
    labels[3]->setPixmap(fimages[3].verticalGaussianBlur(96).toQPixmap());//垂直高斯模糊
    labels[4]->setPixmap(fimages[4].uniformBlur(96).toQPixmap());//均匀模糊
    labels[5]->setPixmap(fimages[5].horizontalUniforBlur(96).toQPixmap());//水平均匀模糊
    labels[6]->setPixmap(fimages[6].verticalUniforBlur(96).toQPixmap());// 垂直均匀模糊
    labels[7]->setPixmap(fimages[7].impulseNoise(0.15).toQPixmap()); // 15%椒盐噪声
    labels[8]->setPixmap(fimages[8].greyScale().toQPixmap());//灰度图

    this->resize(400, 400);
}
