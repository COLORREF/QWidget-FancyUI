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
#include <QRandomGenerator>
#include <opencv2/opencv.hpp>

class FImage
{
public:
    FImage();
    explicit FImage(const QString &fileName, const char *format = nullptr);
    explicit FImage(const QImage &image);
    explicit FImage(QImage &&other) noexcept;
    explicit FImage(const FImage &fimage);
    explicit FImage(FImage &&fimage) noexcept;
    FImage &operator=(const FImage &fimage);
    FImage &operator=(FImage &&fimage) noexcept;
    FImage &operator=(const QImage &qimage);
    FImage &operator=(QImage &&qimage) noexcept;
    operator QImage() const;
    [[nodiscard]] cv::Mat mat() const;

    FImage &gaussianBlur(int radius = 30);           // 高斯模糊
    FImage &horizontalGaussianBlur(int radius = 30); // 水平高斯模糊
    FImage &verticalGaussianBlur(int radius = 30);   // 垂直高斯模糊
    FImage &uniformBlur(int radius = 30);            // 均匀模糊
    FImage &horizontalUniforBlur(int radius = 30);   // 水平均匀模糊
    FImage &verticalUniforBlur(int radius = 30);     // 垂直均匀模糊
    FImage &impulseNoise(double noiseRatio = 0.3);   // 椒盐噪声
    FImage &greyScale();                             // 8bit 单通道 灰度图

    QPixmap toQPixmap() const;
    inline QImage &qImage() { return _qimage; };

    inline void loadMat() { inItMat(); };

private:
    QImage _qimage;
    cv::Mat _mat;
    void inItMat();
};

class [[deprecated("The ImageUtils class is deprecated and the FImage class is recommended")]] ImageUtils
{
public:
    [[deprecated("Function deprecated; the FImage class is recommended")]] static QImage MatToQImage(const cv::Mat &mat);

    [[deprecated("Function deprecated; the FImage class is recommended")]] static cv::Mat QImageToMat(const QImage &image);

    [[deprecated("Function deprecated; the FImage class is recommended")]] static cv::Mat QImageToMat(const QImage &iamge, const char *format);

    [[deprecated("Function deprecated; the FImage class is recommended")]] static cv::Mat LoadResourceImageToMat(const QString &path);

    [[deprecated("Function deprecated; the FImage class is recommended")]] static cv::Mat GaussianBlur(const cv::Mat &MatImg, int radius = 30);

    [[deprecated("Function deprecated; the FImage class is recommended")]] static QImage GaussianBlur(const QImage &img, int radius = 30);

    [[deprecated("Function deprecated; the FImage class is recommended")]] static QPixmap GaussianBlur(const QPixmap &img, int radius = 30);

    [[deprecated("Function deprecated; the FImage class is recommended")]] static QPixmap grabPixmap(QWidget *widget, const QRect &rect);

private:
    ImageUtils();
};

#endif // IMAGEUTILS_H
