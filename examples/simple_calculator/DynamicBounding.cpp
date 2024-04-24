#include "DynamicBounding.h"

DynamicBounding::DynamicBounding(QWidget* parent)
    : QFrame(parent), timer(new QTimer(this))
{
    timer->setInterval(timeInterval);
    connect(timer, &QTimer::timeout, this, [=] { update(); });
}

DynamicBounding::~DynamicBounding()
{
    delete timer;
}

void DynamicBounding::setBackgroundColor(const QColor& bkcolor)
{
    backgroundcolor = bkcolor;
    setStyleSheet("background-color:rgba(" +
                  QString::number(bkcolor.red()) + "," +
                  QString::number(bkcolor.green()) + "," +
                  QString::number(bkcolor.blue()) + "," +
                  QString::number(bkcolor.alpha()) + ");");
}

void DynamicBounding::setBorderRadius(int radius_)
{
    radius = radius_;
    setStyleSheet("*{border-radius:" + QString::number(radius_) + "px;}");
}

void DynamicBounding::setStyleSheet(const QString& style)
{
    QFrame::setStyleSheet(style);
    static QRegularExpression re("border-radius:\\s*(\\d+)px");
    re.match(styleSheet()).hasMatch();
    radius = re.match(styleSheet()).captured(1).toInt();
}

void DynamicBounding::setSpeed(int time)
{
    timer->setInterval(time);
}

void DynamicBounding::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), radius, radius);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setClipPath(path);
    painter.translate(width() / 2, height() / 2);

    if(timer->isActive())
    {
        painter.setBrush(dynamic_color[0]);
        painter.rotate(angle++);
        painter.drawRect(0, 0, width(), height());

        painter.setBrush(dynamic_color[1]);
        painter.rotate(180);
        painter.drawRect(0, 0, width(), height());
    }
    if (angle == 360)
        angle = 0;
}

void DynamicBounding::enterEvent(QEnterEvent *event)
{
    QFrame::enterEvent(event);
    timer->start();
}

void DynamicBounding::leaveEvent(QEvent *event)
{
    QFrame::leaveEvent(event);
    timer->stop();
    update();
}

int DynamicBounding::getBorderRadius()
{
    static QRegularExpression re("border-radius:\\s*(\\d+)px");
    re.match(styleSheet()).hasMatch();
    radius = re.match(styleSheet()).captured(1).toInt();
    return radius;
}

void DynamicBounding::setDynamicColor(const QBrush& dycolor1, const QBrush& dycolor2)
{
    dynamic_color[0] = dycolor1;
    dynamic_color[1] = dycolor2;
}
