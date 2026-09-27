//
// Created by TX on 2025/10/30.
//

#ifndef QWIDGET_FANCYUI_BLURCARD_H
#define QWIDGET_FANCYUI_BLURCARD_H
#include <QPushButton>
#include <QImage>

namespace fancy
{
    class BlurCard : public QPushButton
    {
        Q_OBJECT

    public:
        explicit BlurCard(QWidget *parent);

        /**
         * @param parent 父类
         * @param blurredObj 被模糊对象
         */
        explicit BlurCard(QWidget *parent, QWidget *blurredObj);

        void setBlurRadius(const int radius) { _blurRadius = radius; } // 模糊半径使用逻辑单位，截图时按实际采样密度换算
        [[nodiscard]] int blurRadius() const { return _blurRadius; }
        void setResolutionScale(qreal scale); // 原生分辨率为 1.0，低分辨率可用 0.5、0.25等，设置后调用 blur() 重新生成背景
        [[nodiscard]] qreal resolutionScale() const { return _resolutionScale; }
        void setBlurredObj(QWidget *obj) { _blurredObj = obj; }
        void setRadius(const int radius) { _radius = radius; }
        [[nodiscard]] int radius() const { return _radius; }
        [[nodiscard]] QPixmap pixmap() const { return _pixmap; }
        void setPixmap(const QPixmap &pixmap) { _pixmap = pixmap; }

    public slots:
        void blur();

    protected:
        void paintEvent(QPaintEvent *event) override;

        QPixmap _pixmap;
        int _blurRadius;
        int _radius;
        QWidget *_blurredObj;
        qreal _resolutionScale;
        QImage _blurStage1;
        QImage _blurStage2;
        QImage _blurOutput;
    };
} // fancy

#endif //QWIDGET_FANCYUI_BLURCARD_H
