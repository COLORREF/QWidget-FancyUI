#include "acrylicmaterial.h"
#include "utils/imageutils.h"

AcrylicMaterial::AcrylicMaterial(QWidget *device,const QPixmap& pixmap, int _blurRadius,bool isPixmapFixed)
    :QObject(device)
{
    this->_device = device;
    this->_originalImage = pixmap.toImage();
    this->_drawImage = ImageUtils::GaussianBlur(pixmap.toImage().copy(), _blurRadius);
    this->_blurRadius = _blurRadius;
    this->_isPixmapFixed = isPixmapFixed;
    this->_maskLayerColor = QBrush(QColor(242,242,242,154));
}

void AcrylicMaterial::setBlurRadius(int radius)
{
    if (radius == this->_blurRadius)
        return;
    this->_drawImage = ImageUtils::GaussianBlur(this->_originalImage.copy(),radius);
    this->_blurRadius = radius;
    this->_device->update();
}

void AcrylicMaterial::setMaskLayerColor(const QBrush &maskLayerColor)
{
    this->_maskLayerColor = maskLayerColor;
    this->_device->update();
}

void AcrylicMaterial::setClipPath(const QPainterPath &path)
{
    this->_path = path;
    this->_device->update();
}

void AcrylicMaterial::setPixmap(const QPixmap &pixmap)
{
    this->_originalImage = pixmap.toImage();
    this->_drawImage = ImageUtils::GaussianBlur(pixmap.toImage().copy(),_blurRadius);
    this->_device->update();
}

void AcrylicMaterial::paint()
{
    QPainter painter(this->_device);
    painter.setRenderHints(QPainter::Antialiasing);

    if (!this->_path.isEmpty())
        painter.setClipPath(this->_path);

    //绘制图像
    if(_isPixmapFixed)
        painter.drawImage(0, 0, _drawImage);
    else
        painter.drawImage(0,0,_drawImage.scaled(this->_device->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    //绘制遮罩
    painter.fillPath(this->_path,this->_maskLayerColor);
}

