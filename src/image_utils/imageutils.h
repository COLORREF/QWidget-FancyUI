#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H
#include <QImage>
#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QScreen>
#include <QRect>
#include <QApplication>
namespace cv {
class Mat;
}
class ImageUtils
{
public:
    static QImage MatToQImage(const cv::Mat &mat);

    static cv::Mat QImageToMat(const QImage &image);

    static cv::Mat QImageToMat(const QImage& iamge, const char* format);

    static cv::Mat LoadResourceImageToMat(const QString& path);

    static cv::Mat GaussianBlur(const cv::Mat &MatImg ,int radius = 30);

    static QImage GaussianBlur(const QImage &img ,int radius = 30);

    static QPixmap GaussianBlur(const QPixmap &img ,int radius = 30);

    static QPixmap grabPixmap(QWidget *widget, const QRect &rect);
private:
    ImageUtils();
};

#endif // IMAGEUTILS_H
