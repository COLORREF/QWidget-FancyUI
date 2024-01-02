#include "DynamicLineEdit.h"

DynamicLineEdit::DynamicLineEdit(QWidget *parent)
    : QLineEdit{parent},
      timer{new QTimer(this)}
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->timer->setInterval(msec);
    this->connect(timer, &QTimer::timeout, this, &DynamicLineEdit::inFocus);
    this->setStyleSheet(R"(DynamicLineEdit{
                            background-color:rgba(255,255,255,0);
                            border-radius:10px;
                            padding-left:10px;
                        })");
}
DynamicLineEdit_1::DynamicLineEdit_1(const QString &text, QWidget *parent)
    : DynamicLineEdit_1::DynamicLineEdit_1(parent)
{
    this->setText(text);
}

void DynamicLineEdit::setLineColor(QColor focusOutcolor, QColor focusIncolor)
{
    this->line_color_1 = focusOutcolor;
    this->line_color_2 = focusIncolor;
}

void DynamicLineEdit::setFontColor(QColor fontColor)
{
    QPalette fontcolor(this->palette());
    fontcolor.setColor(QPalette::Text, fontColor);
    this->setPalette(fontcolor);
}

void DynamicLineEdit::setLineSpeed(int speed)
{
    this->msec = speed;
}

void DynamicLineEdit::setLineHeight(int lineHeight)
{
    this->line_height = lineHeight;
}

void DynamicLineEdit::inFocus()
{
    this->line_width_anim += this->line_width_offset;
    if (this->line_width_anim > this->width())
        this->timer->stop();
    this->update();
}

void DynamicLineEdit::outFocus()
{
    this->line_width_anim -= this->line_width_offset;
    if (this->line_width_anim < 0)
        this->timer->stop();
    this->update();
}

void DynamicLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    QPainterPath path = QPainterPath();
    path.addRoundedRect(this->rect(),
                        this->border_radius,
                        this->border_radius); // 添加圆角矩形区域

    QPainter painter = QPainter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing); // 抗锯齿

    painter.setPen(QPen(this->line_color_1, this->line_height, Qt::PenStyle::SolidLine)); // 设置画笔颜色和线条样式
    painter.drawLine(
        QPointF(0, this->height() - this->line_height),
        QPointF(this->width(), this->height() - this->line_height)); // 在底部画未选中时的线条

    painter.setPen(QPen(this->line_color_2, this->line_height, Qt::PenStyle::SolidLine));
    painter.drawLine(
        QPointF(-2, this->height() - this->line_height),
        QPointF(this->line_width_anim, this->height() - this->line_height)); // 在底部画选中时的线条
}

void DynamicLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    this->timer->disconnect();
    this->connect(timer, &QTimer::timeout, this, &DynamicLineEdit::inFocus);
    this->timer->start();
}

void DynamicLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    this->timer->disconnect();
    this->connect(timer, &QTimer::timeout, this, &DynamicLineEdit::outFocus);
    this->timer->start();
}
