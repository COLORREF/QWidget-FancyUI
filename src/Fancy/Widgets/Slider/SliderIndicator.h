//
// Created by TX on 2025/9/26.
//

#ifndef QWIDGET_FANCYUI_SLIDERINDICATOR_H
#define QWIDGET_FANCYUI_SLIDERINDICATOR_H

#include <QWidget>
class QVariantAnimation;

namespace fancy
{
    class SliderIndicator : public QWidget
    {
        friend class SliderStyle;

    public:
        explicit SliderIndicator(QWidget *parent);

    private:
        Qt::Orientation _orientation = Qt::Orientation::Horizontal;
        QVariantAnimation *_ani = nullptr;
        qreal _r;
        qreal _aniValue;
        QPointF _center;
        bool _isContains = false;

    protected:
        void enterEvent(QEnterEvent *event) override;

        void leaveEvent(QEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void paintEvent(QPaintEvent *) override;
    };
} // fancy

#endif //QWIDGET_FANCYUI_SLIDERINDICATOR_H
