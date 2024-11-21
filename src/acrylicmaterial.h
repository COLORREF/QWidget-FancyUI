#ifndef ACRYLICMATERIAL_H
#define ACRYLICMATERIAL_H

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
#include "FancyImage.h"

class AcrylicMaterial: public QObject
{
public:
    AcrylicMaterial(QWidget *device,const QPixmap& pixmap,int blurRadius = 48,bool isPixmapFixed = false);


private:
    QWidget* _device;// 绘图设备兼父类
    int _blurRadius; // 高斯模糊半径
    QBrush _maskLayerColor;//遮罩画刷
    QPainterPath _path;//绘制路径
    QImage _originalImage;//原始图像
    FImage _drawImage;//高斯模糊后图像
    bool _isPixmapFixed;

public:
    void setBlurRadius(int radius);
    void setMaskLayerColor(const QBrush &maskLayer);
    void setClipPath(const QPainterPath &path);
    void setPixmap(const QPixmap& pixmap);
    void paint();
};
#endif // ACRYLICMATERIAL_H
