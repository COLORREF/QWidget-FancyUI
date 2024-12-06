#ifndef FANCYPROGRESSBAR_H
#define FANCYPROGRESSBAR_H

#include <QObject>
#include <QProgressBar>
#include <QWidget>
#include <QPainterPath>
#include "FancyAnimation.h"

class UncertainProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit UncertainProgressBar(QWidget *parent = nullptr);
    void setDuration(int msecs);
    void setEasingCurve(const QEasingCurve &easing);
    int duration()const;
    QEasingCurve easingCurve()const;
protected:
    virtual void paintEvent(QPaintEvent *) override;
    virtual void resizeEvent(QResizeEvent* event) override;
private:
    SimpleAnimation* _ani;
    qreal _lineWidth = 0.0;
    QPainterPath _path;
    qreal _heightRatio = 0.278;
    qreal _height;
};

#endif // FANCYPROGRESSBAR_H
