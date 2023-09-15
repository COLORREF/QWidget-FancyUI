#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    img.load(":/eromanga.png");
    constimg = img;

    //图片边界像素没有处理
    connect(ui->horizontalSlider,&QSlider::valueChanged,this,
            [=](int n)
            {
                if(n == 0)
                {
                    img = constimg;
                    return;
                }
                img = constimg;
                qreal temp_1 = Gaussian1D(-1,1.5);
                qreal temp0 = Gaussian1D(0,1.5);
                qreal temp1 = Gaussian1D(1,1.5);
                qreal sum = temp_1 + temp0 + temp1;
                temp_1 = temp_1/sum;
                temp0 = temp0/sum;
                temp1 = temp1/sum;

                for(int i = 0;i <n;i++)
                {
                    for(int y = 0;y < img.height()-2;y++)
                    {
                        for(int x = 0;x < img.width()-2;x++)
                        {
                            QRgb center = img.pixel(x + 1, y);//中心像素点
                            QRgb Le = img.pixel(x,y);//左侧像素点
                            QRgb Ri = img.pixel(x+2,y);//右侧像素点

                            int red = qRed(Le)*temp_1+qRed(center)*temp0+qRed(Ri)*temp1;

                            int green = qGreen(Le)*temp_1+qGreen(center)*temp0+qGreen(Ri)*temp1;

                            int blue = qBlue(Le)*temp_1+qBlue(center)*temp0+qBlue(Ri)*temp1;

                            img.setPixel(x+1,y,qRgb(red,green,blue));
                        }
                    }

                    for(int x = 0;x < img.width()-2;x++)
                    {
                        for(int y = 0;y <img.height()-2;y++)
                        {
                            QRgb center = img.pixel(x, y+1);//中心像素点
                            QRgb Le = img.pixel(x,y);//左侧像素点
                            QRgb Ri = img.pixel(x,y+2);//右侧像素点

                            int red = qRed(Le)*temp_1+qRed(center)*temp0+qRed(Ri)*temp1;

                            int green = qGreen(Le)*temp_1+qGreen(center)*temp0+qGreen(Ri)*temp1;

                            int blue = qBlue(Le)*temp_1+qBlue(center)*temp0+qBlue(Ri)*temp1;

                            img.setPixel(x,y+1,qRgb(red,green,blue));
                        }
                    }
                }
                update();
    });


    resize(img.width(),img.height());

    //img = GaussiamBlur(50,1.5,img);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);

    painter.drawImage(0,0,img);


}


qreal Gaussian1D(qreal x, qreal o)//x：模糊半径，o：方差
{
    return (qExp(-(qPow(x,2)/(2*qPow(o,2)))))/(o*2*M_PI);//返回1维高斯函数的运算结果
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//下面这个函数可以实现高斯模糊，但是有点问题，up能力不足😭，找不出问题在哪里，如果知道问题出在哪，并且知道怎么解决的话，可以告诉up吗，万分感谢😘
//定义在widget.h里，作为全局函数
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//QImage GaussiamBlur(qreal x, qreal o, QImage& img)//参数说明：模糊半径x，方差o，img图像
//{
//    if(x == 0)
//    {
//        return img;
//    }

//    int imgw = img.width();
//    int imgh = img.height();

//    //根据模糊半径提前计算对应像素权重
//    qreal weightsum = 0.0;
//    QVector<qreal> weight(1+x);
//    for(int i = 0; i < weight.length(); i++)//左右对称，只计算一侧权重
//    {
//        weight[i] = Gaussian1D(i,o);
//        weightsum += weight[i];
//    }
//    weightsum = weightsum*2 - weight[0];//只有一侧的值，所以乘以2减中心值
//    //权重和归一化
//    for(int i = 0; i < weight.length(); i++)
//    {
//        weight[i] /= weightsum;
//    }

//    //二维数组，用于存储原始像素点RGBA值
//    QVector<QVector<QColor>> pixel(imgw,QVector<QColor>(imgh));
//    for(int y_ = 0; y_ < imgh; y_++)
//    {
//        for(int x_ = 0; x_ < imgw; x_++)
//        {
//            QColor color(img.pixel(x_,y_));
//            pixel[x_][y_] = color;
//        }
//    }


//    //用一维高斯函数处理两次，降低时间复杂度，边缘处理忽略，但在一个方向上至少会处理一次

//    QImage blurredImg = img;// 创建一个临时图像来存储模糊处理后的像素值，因为原像素点需要一直以初始值参与运算
//    qreal newR = 0.0;
//    qreal newG = 0.0;
//    qreal newB = 0.0;
//    qreal newA = 0.0;

//    //横向
//    for(int y_ = 0; y_ < imgh; y_++)
//    {
//        for(int x_ = 0; x_ < imgw - x; x_++)
//        {
//            newR = 0.0;
//            newG = 0.0;
//            newB = 0.0;
//            newA = 0.0;

//            // 中心像素点
//            qreal cR = pixel[x_ + x][y_].red() * weight[0];
//            qreal cG = pixel[x_ + x][y_].green() * weight[0];
//            qreal cB = pixel[x_ + x][y_].blue() * weight[0];
//            qreal cA = pixel[x_ + x][y_].alpha() * weight[0];
//            blurredImg.setPixel(x_ + x, y_, qRgba(cR, cG, cB,cA)); // 存储中心像素

//            // 右侧像素点
//            for (int i = 1; i < x + 1; i++)
//            {
//                int rightX = x_ + x + i;
//                if (rightX < imgw ) // 检查是否超出图像边界
//                {
//                    qreal R = pixel[rightX][y_].red() * weight[i];
//                    qreal G = pixel[rightX][y_].green() * weight[i];
//                    qreal B = pixel[rightX][y_].blue() * weight[i];
//                    qreal A = pixel[rightX][y_].alpha() * weight[i];

//                    // 左侧像素点和右侧像素点对称,直接乘以2即可
//                    newR += (2 * R);
//                    newG += (2 * G);
//                    newB += (2 * B);
//                    newA += (2 * A);
//                }
//            }
//            QColor currentColor = blurredImg.pixel(x_ + x, y_);
//            newR += currentColor.red();
//            newG += currentColor.green();
//            newB += currentColor.blue();
//            newA += currentColor.alpha();
//            blurredImg.setPixel(x_ + x, y_, qRgba(newR, newG, newB,newA)); // 加权平均，即模糊后的像素点的颜色值
//        }
//    }

//    //纵向
//    for(int x_ = 0; x_ < imgh; x_++)
//    {
//        for(int y_ = 0; y_ < imgh - x; y_++)
//        {
//            newR = 0.0;
//            newG = 0.0;
//            newB = 0.0;
//            newA = 0.0;

//            // 中心像素点
//            qreal cR = pixel[y_ + x][x_].red() * weight[0];
//            qreal cG = pixel[y_ + x][x_].green() * weight[0];
//            qreal cB = pixel[y_ + x][x_].blue() * weight[0];
//            qreal cA = pixel[y_ + x][x_].alpha() * weight[0];

//            blurredImg.setPixel(y_ + x, x_, qRgba(cR, cG, cB,cA)); // 存储中心像素

//            // 下方像素点
//            for (int i = 1; i < x + 1; i++)
//            {
//                int rightX = y_ + x + i;
//                if (rightX < imgh ) // 检查是否超出图像边界
//                {
//                    qreal R = pixel[rightX][x_].red() * weight[i];
//                    qreal G = pixel[rightX][x_].green() * weight[i];
//                    qreal B = pixel[rightX][x_].blue() * weight[i];
//                    qreal A = pixel[rightX][x_].alpha() * weight[i];

//                    // 下方像素点和上方像素点对称,直接乘以2即可

//                    newR += (2 * R);
//                    newG += (2 * G);
//                    newB += (2 * B);
//                    newA += (2 * A);
//                }
//            }

//            QColor currentColor = blurredImg.pixel(y_ + x, x_);
//            newR += currentColor.red();
//            newG += currentColor.green();
//            newB += currentColor.blue();
//            newA += currentColor.alpha();
//            blurredImg.setPixel(y_ + x, x_, qRgba(newR, newG, newB,newA)); // 加权平均，即模糊后的像素点的颜色值
//        }
//    }

//    img = blurredImg;// 将模糊处理后的图像复制回原始图像
//    return img;
// }















