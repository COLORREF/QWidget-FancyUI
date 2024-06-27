#ifndef ACRYLICBRUSH_H
#define ACRYLICBRUSH_H

#include <QObject>
#include <QWidget>
#include <QColor>
#include <QPainterPath>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QRect>
#include <QPaintDevice>

class AcrylicBrush : public QObject
{
public:
    AcrylicBrush(QWidget *device,
                 const QPixmap& pixmap,
                 int blurRadius = 48,
                 bool isPixmapFixed = false);

private:
    QWidget *device; // 绘图设备兼父类
    int blurRadius; // 高斯模糊半径
    QBrush mask_layer_color;//遮罩画刷
    QPainterPath path;//绘制路径
    QImage draw_image;//高斯模糊后图像
    QImage originalImage;//原始图像
    bool is_pixmap_fixed = false;
public:
    void setBlurRadius(int radius);
    void setMaskLayerColor(const QBrush &maskLayer);
    void setClipPath(const QPainterPath &path);
    void setPixmap(const QPixmap& pixmap);
    void paint();
};
#endif // ACRYLICBRUSH_H
