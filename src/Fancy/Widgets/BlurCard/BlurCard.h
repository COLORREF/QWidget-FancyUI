//
// Created by TX on 2025/10/30.
//

#ifndef QWIDGET_FANCYUI_BLURCARD_H
#define QWIDGET_FANCYUI_BLURCARD_H
#include <QPushButton>

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

        void setBlurRadius(int radius) { _blurRadius = radius; }
        [[nodiscard]] int blurRadius() const { return _blurRadius; }
        void setBlurredObj(QWidget *obj) { _blurredObj = obj; }
        void setRadius(int radius) { _radius = radius; }
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
    };
} // fancy

#endif //QWIDGET_FANCYUI_BLURCARD_H
