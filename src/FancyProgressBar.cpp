#include "FancyProgressBar.h"
#include "FancyStyle.h"

UncertainProgressBar::UncertainProgressBar(QWidget *parent):
    QProgressBar(parent)
{
    this->_ani = new SimpleAnimation(0.0,0.0,2200,true,this,this);
    connect(this->_ani,&SimpleAnimation::finished,this,[ani = this->_ani](){ani->start();});
}

void UncertainProgressBar::setDuration(int msecs)
{
    this->_ani->setDuration(msecs);
}

void UncertainProgressBar::setEasingCurve(const QEasingCurve &easing)
{
    this->_ani->setEasingCurve(easing);
}

int UncertainProgressBar::duration()const
{
    return this->_ani->duration();
}

QEasingCurve UncertainProgressBar::easingCurve()const
{
    return this->_ani->easingCurve();
}

void UncertainProgressBar::paintEvent(QPaintEvent *)
{
    QRect rect = this->rect();
    qreal center = rect.center().y() + 0.5;//修正视觉误差
    qreal x = this->_ani->_runTimeValue.toReal();
    auto colors = ControlColors::controlColors();
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setClipPath(_path);
    painter.fillRect(this->rect(),colors->normalBorder());
    QLineF primaryLine(x,center,x+this->_lineWidth,center);
    painter.setPen(QPen(colors->theme(),_height ,Qt::PenStyle::SolidLine,Qt::PenCapStyle::RoundCap));
    painter.drawLine(primaryLine);
}

void UncertainProgressBar::resizeEvent(QResizeEvent *event)
{
    QProgressBar::resizeEvent(event);
    this->_lineWidth = this->width()/4.7;
    QRect rect = this->rect();
    _height =  rect.height() * _heightRatio;
    qreal y = (rect.height()- _height)/2;
    QRectF background(rect.x(),y,rect.width(),rect.height() * _heightRatio);
    qreal r = background.height()/2;
    _path.clear();
    _path.addRoundedRect(background,r,r);
    this->_ani->setStartValue(-this->_lineWidth);
    this->_ani->setEndValue((qreal)(this->width()));
    if(this->_ani->state() == QVariantAnimation::State::Stopped)
        this->_ani->start();
}
