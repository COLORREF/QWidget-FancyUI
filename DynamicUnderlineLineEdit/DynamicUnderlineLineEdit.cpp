#include "DynamicUnderlineLineEdit.h"

DynamicUnderlineLineEdit::DynamicUnderlineLineEdit(QWidget *parent)
    : QLineEdit{parent},
      timer{new QTimer(this)}
{
    this->setFocusPolicy(Qt::ClickFocus);
    this->timer->setInterval(msec);
    this->connect(timer, &QTimer::timeout, this, &DynamicUnderlineLineEdit::inFocus);
    this->setStyleSheet(R"(DynamicUnderlineLineEdit{
                            background-color:rgba(255,255,255,0);
                            border-radius:10px;
                            padding-left:10px;
                        })");
}
DynamicUnderlineLineEdit::DynamicUnderlineLineEdit(const QString &text, QWidget *parent)
    : DynamicUnderlineLineEdit::DynamicUnderlineLineEdit(parent)
{
    this->setText(text);
}

void DynamicUnderlineLineEdit::setLineColor(const QColor &focusOutcolor, const QColor &focusIncolor)
{
    this->line_color_1 = focusOutcolor;
    this->line_color_2 = focusIncolor;
}

void DynamicUnderlineLineEdit::setFontColor(const QColor &fontColor)
{
    const_cast<QPalette &>(this->palette()).setColor(QPalette::ColorRole::Text, fontColor);
}

void DynamicUnderlineLineEdit::setPlaceholderTextColor(const QColor &placeholderText)
{
    const_cast<QPalette &>(this->palette()).setColor(QPalette::ColorRole::PlaceholderText, placeholderText);
}

void DynamicUnderlineLineEdit::setLineSpeed(int speed)
{
    this->msec = speed;
}

void DynamicUnderlineLineEdit::setLineHeight(int lineHeight)
{
    this->line_height = lineHeight;
}

void DynamicUnderlineLineEdit::inFocus()
{
    this->line_width_anim += this->line_width_offset;
    if (this->line_width_anim > this->width())
        this->timer->stop();
    this->update();
}

void DynamicUnderlineLineEdit::outFocus()
{
    this->line_width_anim -= this->line_width_offset;
    if (this->line_width_anim < 0)
        this->timer->stop();
    this->update();
}

void DynamicUnderlineLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

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

void DynamicUnderlineLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    this->timer->disconnect();
    this->connect(timer, &QTimer::timeout, this, &DynamicUnderlineLineEdit::inFocus);
    this->timer->start();
}

void DynamicUnderlineLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    this->timer->disconnect();
    this->connect(timer, &QTimer::timeout, this, &DynamicUnderlineLineEdit::outFocus);
    this->timer->start();
}
