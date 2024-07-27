#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H
#include <QImage>
#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QScreen>
#include <QRect>
#include <QApplication>
#include <QtMath>
#include <QBuffer>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <opencv2/opencv.hpp>

class FImage
{
public:
    FImage();
    explicit FImage(const QString& fileName, const char *format = nullptr);
    explicit FImage(const QImage& image);
    FImage(QImage &&other) noexcept;
    FImage(const FImage& fimage);
    FImage(FImage&& fimage) noexcept;
    FImage &operator=(const FImage & fimage);
    FImage &operator=(FImage&& fimage) noexcept;
    operator QImage();
    [[nodiscard]] cv::Mat mat() const;
    FImage& GaussianBlur(int radius = 30);
    QPixmap toQPixmap();
    QImage& qImage();
private:
    QImage _qimage;
    cv::Mat _mat;
    void inItMat();
};

class[[deprecated("The ImageUtils class is deprecated and the FImage class is recommended")]] ImageUtils
{
public:
    [[deprecated("Function deprecated; the FImage class is recommended")]]static QImage MatToQImage(const cv::Mat &mat);

    [[deprecated("Function deprecated; the FImage class is recommended")]]static cv::Mat QImageToMat(const QImage &image);

    [[deprecated("Function deprecated; the FImage class is recommended")]]static cv::Mat QImageToMat(const QImage& iamge, const char* format);

    [[deprecated("Function deprecated; the FImage class is recommended")]]static cv::Mat LoadResourceImageToMat(const QString& path);

    [[deprecated("Function deprecated; the FImage class is recommended")]]static cv::Mat GaussianBlur(const cv::Mat &MatImg ,int radius = 30);

    [[deprecated("Function deprecated; the FImage class is recommended")]]static QImage GaussianBlur(const QImage &img ,int radius = 30);

    [[deprecated("Function deprecated; the FImage class is recommended")]]static QPixmap GaussianBlur(const QPixmap &img ,int radius = 30);

    [[deprecated("Function deprecated; the FImage class is recommended")]]static QPixmap grabPixmap(QWidget *widget, const QRect &rect);
private:
    ImageUtils();
};

#endif // IMAGEUTILS_H
