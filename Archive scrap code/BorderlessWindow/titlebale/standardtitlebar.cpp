#include "standardtitlebar.h"

StandardTitleBar::StandardTitleBar(QWidget *parent)
    : SimpleTitleBar{parent},
    icon_label{new QLabel(this)},
    title_label{new TitleTextLabel(this)}

{
    this->horizontal_layout->setContentsMargins(8, 0, 0, 0);
    this->icon_label->setFixedSize(25, 20);
    this->icon_label->setScaledContents(true);
    this->icon_label->setContentsMargins(0, 0, 5, 0);
    this->title_label->setFont(QFont("Microsoft YaHei", 9));
    this->horizontal_layout->insertWidget(0, title_label);
    this->horizontal_layout->insertWidget(0, icon_label);

    QPalette p(this->title_label->palette());
    p.setBrush(QPalette::ColorRole::WindowText,Qt::GlobalColor::black);
    this->title_label->setPalette(p);

    // 设置默认图标和标题
    icon_label->setPixmap(this->style()->standardIcon(QStyle::StandardPixmap::SP_ComputerIcon).pixmap(20, 20));
    title_label->setText(QApplication::applicationName());
}

void StandardTitleBar::setIcon(const QPixmap &icon)
{
    this->icon_label->setPixmap(icon);
}

void StandardTitleBar::setTitle(const QString &title)
{
    this->title_label->setText(title);
}

void StandardTitleBar::setTitleFont(const QFont &font)
{
    this->title_label->setFont(font);
}

void StandardTitleBar::setTitleColor(const QColor &titleColor)
{
    this->titlt_text_color = titleColor;
    this->title_label->update();
}

QWidget *StandardTitleBar::iconLabel() const
{
    return this->icon_label;
}

StandardTitleBar::TitleTextLabel *StandardTitleBar::titleLabel() const
{
    return this->title_label;
}

void StandardTitleBar::TitleTextLabel::setIsClearBeforeNewPaint(bool isClear)
{
    this->is_clear_before_new_paint = isClear;
}

void StandardTitleBar::TitleTextLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::TextAntialiasing);
    if (this->is_clear_before_new_paint)
    {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
    }
    QLabel::paintEvent(event);
}
