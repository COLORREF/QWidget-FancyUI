#include "mypushbutton_QPushButton.h"

mypushbutton_QPushButton::mypushbutton_QPushButton(QWidget* parent)
    : QPushButton(parent), timer(new QTimer(this)), fill_color(qRgb(255, 89, 0))
{
    timer->setInterval(timeInterval);
    max_radius = qSqrt(width() * width() + height() * height());
}

void mypushbutton_QPushButton::setTimeInterval(const int timeinitval)
{
    timer->setInterval(timeinitval);
}

void mypushbutton_QPushButton::setText(const QString& text, const QFont& font, const QColor& fontcolor)
{
    setFont(font);
    QPushButton::setText(text);
    QPushButton::setStyleSheet("color: rgb(" +
                               QString::number(fontcolor.red()) + "," +
                               QString::number(fontcolor.green()) + "," +
                               QString::number(fontcolor.blue()) + ")");
}

void mypushbutton_QPushButton::setFillColor(const QColor& fillcolor)
{
    fill_color = fillcolor;
}

void mypushbutton_QPushButton::resize(const QSize& size)
{
    QPushButton::resize(size);
    max_radius = qSqrt(width() * width() + height() * height());
}

void mypushbutton_QPushButton::resize(const int w, const int h)
{
    QPushButton::resize(w, h);
    max_radius = qSqrt(width() * width() + height() * height());
}

void mypushbutton_QPushButton::setStyleSheet(const QString& style)
{
    QPushButton::setStyleSheet(style);
    QRegularExpression re("border-radius:\\s*(\\d+)px");
    re.match(styleSheet()).hasMatch();
    btn_radius = re.match(styleSheet()).captured(1).toInt();
}

void mypushbutton_QPushButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);
    if (!mouse_point.isNull())
    {
        QPainter painter(this);
        QPainterPath path;
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(fill_color));
        painter.setPen(Qt::NoPen);
        path.addRoundedRect(rect(), btn_radius, btn_radius);
        painter.setClipPath(path);
        painter.drawEllipse(mouse_point, radius, radius);
    }
    return;
}

void mypushbutton_QPushButton::enterEvent(QEnterEvent* event)
{
    mouse_point = event->position();
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, [=] {
        radius += radius_var;
        if (radius > max_radius)
        {
            timer->stop();
            return;
        }
        update();
    });
    timer->start();
}

void mypushbutton_QPushButton::leaveEvent(QEvent*)
{
    mouse_point = mapFromGlobal(QCursor::pos());
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, [=] {
        radius -= radius_var;
        if (radius < 0)
        {
            timer->stop();
            return;
        }
        update();
    });
    timer->start();
}
