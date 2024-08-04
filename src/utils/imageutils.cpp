#include "imageutils.h"

FImage::FImage(){}

FImage::FImage(const QString &fileName, const char *format):
    _qimage{fileName, format}
{
    this->inItMat();
}

FImage::FImage(const QImage &image):
    _qimage{image}
{
    this->inItMat();
}

FImage::FImage(QImage &&other) noexcept:
    _qimage{std::forward<QImage&&>(other)}
{
    this->inItMat();
}

FImage::FImage(const FImage &fimage):
    FImage(fimage._qimage)
{
}

FImage::FImage(FImage &&fimage) noexcept:
    FImage(std::move(fimage._qimage))
{
}

FImage &FImage::operator=(const FImage &fimage)
{
    *this = fimage._qimage;
    return *this;
}

FImage &FImage::operator=(FImage &&fimage) noexcept
{
    *this = std::move(fimage._qimage);
    return *this;
}

FImage &FImage::operator=(const QImage &qimage)
{
    this->_qimage = qimage;
    this->inItMat();
    return *this;
}

FImage &FImage::operator=(QImage&& qimage)noexcept
{
    /*外部调用等效于：
    fimage.qImage() = qimage.copy();
    fimage.loadMat();
    */
    this->_qimage = std::forward<QImage&&>(qimage);
    this->inItMat();
    return *this;
}

FImage::operator QImage() const
{
    return _qimage;
}

cv::Mat FImage::mat() const
{
    return this->_mat.clone();
}

FImage& FImage::gaussianBlur(int radius)
{
    if(radius > 0)
    {
        int ksize = radius * 2 + 1;
        cv::GaussianBlur(_mat, _mat, cv::Size(ksize,ksize), 0, 0);
    }
    else if(radius < 0)
        qWarning()<<"The blur radius must be greater than or equal to 0";

    return *this;
}

FImage &FImage::horizontalGaussianBlur(int radius)
{
    if(radius > 0)
        cv::GaussianBlur(_mat, _mat, cv::Size(radius * 2 + 1,1), 0, 0);
    else if(radius < 0)
        qWarning()<<"The blur radius must be greater than 0";
    return *this;
}

FImage &FImage::verticalGaussianBlur(int radius)
{
    if(radius > 0)
        cv::GaussianBlur(_mat, _mat, cv::Size(1,radius * 2 + 1), 0, 0);
    else if(radius < 0)
        qWarning()<<"The blur radius must be greater than or equal to 0";
    return *this;
}

FImage &FImage::uniformBlur(int radius)
{
    if(radius > 0)
        cv::blur(_mat, _mat, cv::Size(radius, radius));
    if(radius < 0)
        qWarning()<<"The blur radius must be greater than 0";
    return *this;
}

FImage &FImage::horizontalUniforBlur(int radius)
{
    if(radius > 0)
        cv::filter2D(_mat, _mat, -1, cv::Mat::ones(1, radius, CV_32F) / static_cast<float>(radius));
    if(radius < 0)
        qWarning()<<"The blur radius must be greater than 0";
    return *this;
}

FImage &FImage::verticalUniforBlur(int radius)
{
    if(radius > 0)
        cv::filter2D(_mat, _mat, -1, cv::Mat::ones(radius, 1, CV_32F) / static_cast<float>(radius));
    if(radius < 0)
        qWarning()<<"The blur radius must be greater than 0";
    return *this;
}

FImage &FImage::impulseNoise(double noiseRatio)
{
    QRgb white = qRgba(255,255,255,255);
    QRgb black = qRgba(0,0,0,255);
    if(noiseRatio > 0.0 && noiseRatio < 1.0)
    {
        for(int x = 0; x< _qimage.width(); x++)
            for(int y = 0; y<_qimage.height(); y++)
                if(QRandomGenerator::global()->bounded(1.0) <= noiseRatio) //命中测试 [0,1.0)
                    _qimage.setPixel(x, y, QRandomGenerator::global()->bounded(0, 2) ? black : white);
    }
    else if(noiseRatio >= 1.0)
    {
        for(int x = 0; x< _qimage.width(); x++)
            for(int y = 0; y<_qimage.height(); y++)
                _qimage.setPixel(x, y, QRandomGenerator::global()->bounded(0, 2) ? black : white);
    }
    else if(noiseRatio < 0.0)
        qWarning()<<"Noise ratio must be greater than or equal to 0";

    return *this;
}

FImage &FImage::greyScale()
{
    _qimage = _qimage.convertToFormat(QImage::Format_Grayscale8);
    _mat = cv::Mat(_mat.rows,_mat.cols, CV_8UC1, (void*)(_qimage.constBits()), _qimage.bytesPerLine());
    return *this;
}
QPixmap FImage::toQPixmap()const
{
    return QPixmap::fromImage(_qimage);
}

void FImage::inItMat()
{
    switch(this->_qimage.format())
    {
    case QImage::Format_Grayscale8: // 8-bit, 1 channel 灰度图
        _mat = cv::Mat(_qimage.height(),_qimage.width(),CV_8UC1, (void*)(_qimage.constBits()),_qimage.bytesPerLine());
        break;
    case QImage::Format_Grayscale16:// 16-bit, 1 channel 灰度图
        _mat = cv::Mat(_qimage.height(),_qimage.width(),CV_16UC1, (void*)(_qimage.constBits()),_qimage.bytesPerLine());
        break;
    case QImage::Format_ARGB32: // uint32存储0xAARRGGBB，pc一般小端存储低位在前，实际字节顺序就成了BGRA
        [[fallthrough]];
    case QImage::Format_RGB32: // Alpha为FF
        [[fallthrough]];
    case QImage::Format_ARGB32_Premultiplied:
        _mat = cv::Mat(_qimage.height(),_qimage.width(),CV_8UC4, (void*)(_qimage.constBits()),_qimage.bytesPerLine());
        break;
    case QImage::Format_RGB888: // 8-bit, 3 channel，RGB顺序
        _mat = cv::Mat(_qimage.height(),_qimage.width(),CV_8UC3, (void*)(_qimage.constBits()),_qimage.bytesPerLine());
        cv::cvtColor(this->_mat, this->_mat, cv::COLOR_RGB2BGR);// opencv以BGR顺序存储，需要转换顺序
        break;
    case QImage::Format_RGBA64: // uint64存储，顺序和Format_ARGB32相反，RGBA
        _mat = cv::Mat(_qimage.height(),_qimage.width(),CV_16UC4, (void*)(_qimage.constBits()),_qimage.bytesPerLine());
        cv::cvtColor(this->_mat, this->_mat, cv::COLOR_RGBA2BGRA);// opencv以BGRA顺序存储，需要转换顺序
        break;

    case QImage::Format_Mono://1 bit, 1 channel 黑白位图
    {
        // opencv 无法通过内存数据直接加载此格式图片
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        _qimage.save(&buffer, "jpg");
        buffer.close();
        _qimage = QImage::fromData(byteArray);
        this->inItMat();
    }
    break;

    default:
        qWarning()<<"QImage type not handled in switch:"<<this->_qimage.format();
        break;
    }
}


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
        qWarning()<<"The blur radius must be greater than or equal to 0";
        return MatImg;
    }
    int ksize = radius * 2 + 1;
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
