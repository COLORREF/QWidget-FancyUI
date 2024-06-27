#include "imageutils.h"
#include <vector>
#include <QtMath>
#include <QBuffer>
#include <QFile>
#include <QByteArray>
#include <QDebug>
// #include "opencv2/opencv.hpp"
#include "opencv2/core/hal/interface.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc.hpp"

QImage ImageUtils::MatToQImage(const cv::Mat &mat)
{
    // 判断图像类型
    switch (mat.type())
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy(); // 必须返回拷贝，否则 mat销毁后 image内数据不可访问，程序崩溃（QImage重载了operator=使用引用参数）
    }
    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped(); // 需要交换RGB通道
    }
    // 8-bit, 1 channel
    case CV_8UC1:
    {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return image.copy();
    }
    default:
        qWarning("cv::Mat image type not handled in switch: %d", mat.type());
        break;
    }

    return QImage();
}

cv::Mat ImageUtils::QImageToMat(const QImage &image)
{
    cv::Mat mat; // Mat构造：行数，列数，存储结构，数据，step每行多少字节
    switch(image.format())
    {
    case QImage::Format_Grayscale8: // 8-bit, 1 channel 灰度图
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_ARGB32: // uint32存储0xAARRGGBB，pc一般小端存储低位在前，实际字节顺序就成了BGRA
    case QImage::Format_RGB32: // Alpha为FF
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888: // 8-bit, 3 channel，RGB顺序
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);// opencv以BGR顺序存储，需要转换顺序
        break;
    case QImage::Format_RGBA64: // uint64存储，顺序和Format_ARGB32相反，RGBA
        mat = cv::Mat(image.height(), image.width(), CV_16UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);// opencv以BGRA顺序存储，需要转换顺序
        break;
    default:
        qWarning("QImage type not handled in switch: ", image.format());
        break;
    }
    return mat;
}

cv::Mat ImageUtils::QImageToMat(const QImage &iamge, const char *format)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    iamge.save(&buffer, format);
    buffer.close();
    return cv::imdecode(std::vector<char>(byteArray.begin(), byteArray.end()), cv::IMREAD_COLOR);
}

cv::Mat ImageUtils::LoadResourceImageToMat(const QString &path)
{
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QByteArray ba = file.readAll();
        file.close();
        return cv::imdecode(std::vector<char>(ba.begin(), ba.end()), cv::IMREAD_COLOR);
    }
    return cv::Mat();
}

cv::Mat ImageUtils::GaussianBlur(const cv::Mat &MatImg, int radius)
{
    if(radius <= 0)
    {
        qWarning()<<"The blur radius must be greater than 0";
        return MatImg;
    }
    int ksize = radius * 2 + 1;
    // // 分割图像通道
    // std::vector<cv::Mat> channels;
    // cv::split(MatImg, channels);

    // //对每个通道进行高斯模糊
    // for(cv::Mat& m : channels)
    //     cv::GaussianBlur(m, m, cv::Size(radius,radius), 0, 0);

    // // 重新合并通道
    // cv::merge(channels, MatImg);

    // double sigma = (0.3* (((ksize-1)*0.5) - 1) + 0.8);
    cv::GaussianBlur(MatImg, MatImg, cv::Size(ksize,ksize), 0, 0);
    return MatImg;
}

QImage ImageUtils::GaussianBlur(const QImage &img, int radius)
{
    return MatToQImage(GaussianBlur(QImageToMat(img),radius));
}

QPixmap ImageUtils::GaussianBlur(const QPixmap &img, int radius)
{
    return QPixmap::fromImage(MatToQImage(GaussianBlur(QImageToMat(img.toImage()),radius)));
}

QPixmap ImageUtils::grabPixmap(QWidget *widget, const QRect &rect)
{
    QScreen *screen = QApplication::screenAt(widget->window()->pos());
    if (!screen)
        screen = QApplication::screens().at(0);
    return screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
}

ImageUtils::ImageUtils() {}
