#include "acrylicmaterial.h"

AcrylicMaterial::AcrylicMaterial(QWidget *device,const QPixmap& pixmap, int blurRadius,bool isPixmapFixed)
    :QObject(device),
    _device(device),
    _blurRadius(blurRadius),
    _maskLayerColor(QColor(242,242,242,154)),
    _originalImage(pixmap.toImage()),
    _drawImage(_originalImage.copy()),
    _isPixmapFixed(isPixmapFixed)
{
    this->_drawImage.gaussianBlur(blurRadius);
}

void AcrylicMaterial::setBlurRadius(int radius)
{
    if (radius == this->_blurRadius)
        return;
    this->_drawImage = this->_originalImage.copy();
    this->_blurRadius = radius;
    this->_drawImage.gaussianBlur(radius);
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
    this->_drawImage = this->_originalImage.copy();
    this->_drawImage.gaussianBlur(this->_blurRadius);
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
        painter.drawImage(0, 0, this->_drawImage);
    else
        painter.drawImage(0,0,this->_drawImage.qImage().scaled(this->_device->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    //绘制遮罩
    painter.fillPath(this->_path,this->_maskLayerColor);
}

