#include "acrylicbrush.h"
#include "../../utils/image_utils/imageutils.h"

AcrylicBrush::AcrylicBrush(QWidget *device,const QPixmap& pixmap, int blurRadius,bool isPixmapFixed)
    :QObject(device)
{
    this->device = device;
    this->originalImage = pixmap.toImage();
    this->draw_image = ImageUtils::GaussianBlur(pixmap.toImage().copy(), blurRadius);
    this->blurRadius = blurRadius;
    this->is_pixmap_fixed = isPixmapFixed;
    this->mask_layer_color = QBrush(QColor(242,242,242,154));
}

void AcrylicBrush::setBlurRadius(int radius)
{
    if (radius == this->blurRadius)
        return;
    this->draw_image = ImageUtils::GaussianBlur(originalImage.copy(),radius);
    this->blurRadius = radius;
    this->device->update();
}

void AcrylicBrush::setMaskLayerColor(const QBrush &maskLayerColor)
{
    this->mask_layer_color = maskLayerColor;
    this->device->update();
}

void AcrylicBrush::setClipPath(const QPainterPath &path)
{
    this->path = path;
    this->device->update();
}

void AcrylicBrush::setPixmap(const QPixmap &pixmap)
{
    this->originalImage = pixmap.toImage();
    this->draw_image = ImageUtils::GaussianBlur(pixmap.toImage().copy(),blurRadius);
    this->device->update();
}

void AcrylicBrush::paint()
{
    QPainter painter(device);
    painter.setRenderHints(QPainter::Antialiasing);

    if (!path.isEmpty())
        painter.setClipPath(path);

    //绘制图像
    if(is_pixmap_fixed)
        painter.drawImage(0, 0, draw_image);
    else
        painter.drawImage(0,0,draw_image.scaled(device->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

    //绘制遮罩
    painter.fillPath(path,mask_layer_color);
}

