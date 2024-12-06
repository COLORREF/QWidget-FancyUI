#ifndef FANCYLABEL_H
#define FANCYLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPixmap>
#include <QtMath>
#include <QGraphicsBlurEffect>


class Label : public QLabel
{
public:
    explicit Label(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit Label(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
};



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

#endif // FANCYLABEL_H
