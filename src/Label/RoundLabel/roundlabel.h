#ifndef ROUNDLABEL_H
#define ROUNDLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPixmap>
#include <QtMath>
#include <QGraphicsBlurEffect>

class RoundLabel : public QLabel
{
    Q_OBJECT
public:
    explicit RoundLabel(QWidget* parent);
    void setPixmap(const QPixmap& pixmap);
    void addGraphicsBlurEffect(qreal radius = 10.0, QGraphicsBlurEffect::BlurHint hints = QGraphicsBlurEffect::BlurHint::QualityHint);
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // ROUNDLABEL_H
