#include "DynamicBorderFrame.h"

DynamicBorderFrame::DynamicBorderFrame(QWidget* parent)
    : QFrame(parent), timer(new QTimer(this))
{
    timer->setInterval(timeInterval);
    connect(timer, &QTimer::timeout, this, [=] { update(); });
}

DynamicBorderFrame::~DynamicBorderFrame()
{
    delete timer;
}

void DynamicBorderFrame::setBackgroundColor(const QColor& bkcolor)
{
    backgroundcolor = bkcolor;
    setStyleSheet("background-color:rgba(" +
                  QString::number(bkcolor.red()) + "," +
                  QString::number(bkcolor.green()) + "," +
                  QString::number(bkcolor.blue()) + "," +
                  QString::number(bkcolor.alpha()) + ");");
}

void DynamicBorderFrame::setBorderRadius(int radius_)
{
    radius = radius_;
    setStyleSheet("*{border-radius:" + QString::number(radius_) + "px;" +
                  "background-color:rgba(" +
                  QString::number(backgroundcolor.red()) + "," +
                  QString::number(backgroundcolor.green()) + "," +
                  QString::number(backgroundcolor.blue()) + "," +
                  QString::number(backgroundcolor.alpha()) + ");}");
}

void DynamicBorderFrame::setStyleSheet(const QString& style)
{
    QFrame::setStyleSheet(style);
    static QRegularExpression re("border-radius:\\s*(\\d+)px");
    re.match(styleSheet()).hasMatch();
    radius = re.match(styleSheet()).captured(1).toInt();
}

void DynamicBorderFrame::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);

    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), radius, radius);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setClipPath(path);
    painter.translate(width() / 2, height() / 2);
    painter.setBrush(dynamic_color[0]);
    painter.rotate(angle++);
    painter.drawRect(0, 0, width(), height());

    painter.setBrush(dynamic_color[1]);
    painter.rotate(180);
    painter.drawRect(0, 0, width(), height());

    if (angle == 360)
        angle = 0;
}

void DynamicBorderFrame::showEvent(QShowEvent* event)
{
    QFrame::showEvent(event);
    timer->start();
}

int DynamicBorderFrame::getBorderRadius()
{
    static QRegularExpression re("border-radius:\\s*(\\d+)px");
    re.match(styleSheet()).hasMatch();
    radius = re.match(styleSheet()).captured(1).toInt();
    return radius;
}

void DynamicBorderFrame::setDynamicColor(const QBrush& dycolor1, const QBrush& dycolor2)
{
    dynamic_color[0] = dycolor1;
    dynamic_color[1] = dycolor2;
}
