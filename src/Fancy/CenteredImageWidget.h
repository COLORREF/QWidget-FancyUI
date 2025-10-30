//
// Created by TX on 2025/10/15.
//

#ifndef QWIDGET_FANCYUI_CENTEREDIMAGEWIDGET_H
#define QWIDGET_FANCYUI_CENTEREDIMAGEWIDGET_H
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QTimer>
#include <QWidget>


namespace fancy
{
    class CenteredImageWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit CenteredImageWidget(QWidget *parent = nullptr, int radius = 10);

        explicit CenteredImageWidget(QWidget *parent, const QPixmap &pixmap, int radius = 10);

        void setPixmap(const QPixmap &pixmap);

        void setRadius(int radius);

        void setDrawBorder(bool drawBorder) { _drawBorder = drawBorder; }

        void setDrawMask(bool drawMask) { _drawMask = drawMask; }

        void setFillBackground(bool fill) { _fillBackground = fill; }

        void setMaskColor(const QColor &color) { _mask = color; }

        void setFillColor(const QColor &color) { _bkFill = color; }

        int radius() const { return _radius; }
        QColor maskColor() const { return _mask; }
        QColor fillColor() const { return _fillBackground; }

    protected:
        void paintEvent(QPaintEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

    private:
        void updateClipPath();

        void updateFastScaledPixmap();

        void updateSmoothScaledPixmap();

        QPixmap _pixmap; // 原图
        QPixmap _scaledFast; // 快速缩放版本
        QPixmap _scaledSmooth; // 平滑缩放版本
        QPainterPath _painterPath;
        QPoint _offset;
        QTimer _debounceTimer; // 防抖定时器
        int _radius = 10;
        bool _drawBorder;
        bool _drawMask;
        bool _fillBackground;
        QColor _mask;
        QColor _bkFill;

    signals:
        void SmoothPixmapUpdate();

        void FastTPixmapUpdate();
    };
} // fancy

#endif //QWIDGET_FANCYUI_CENTEREDIMAGEWIDGET_H
