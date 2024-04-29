#include "sidebaroptionsbutton.h"

SidebarOptionsButton::SidebarOptionsButton(QWidget *parent, int index, int minimumWidth, int fixedHeight)
    : QRadioButton{parent},
      icon_label{new QLabel(this)},
      text_label{new QLabel(this)},
      index_{index}
{
    QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setMinimumWidth(minimumWidth);
    this->setFixedHeight(fixedHeight);
    this->resize(minimumWidth, fixedHeight);

    this->icon_label->resize(size() / 2);
    this->icon_label->move(width() / 4, height() / 4);
    this->icon_label->setScaledContents(true);

    this->text_label->move(this->width() + parent->layout()->contentsMargins().right(), height() / 4);

    connect(this, &SidebarOptionsButton::toggled, this, [&](bool isClicked)
            {
                if(isClicked)
                {
                    emit this->selectedIndex(this->index_);
                    this->background_brush.setColor(this->clicked_color);
                }
                else
                    this->background_brush.setColor(this->dis_clicked_Color);
                this->update();
            });
}

void SidebarOptionsButton::setDisClickedColor(const QColor &color)
{
    this->dis_clicked_Color = color;
}

void SidebarOptionsButton::setClickedColor(const QColor &color)
{
    this->clicked_color = color;
}

void SidebarOptionsButton::setClickedEnterColor(const QColor &color)
{
    this->clicked_enter_color = color;
}

void SidebarOptionsButton::setDisClickedEnterColor(const QColor &color)
{
    this->dis_clicked_enter_color = color;
}

void SidebarOptionsButton::setPromptLineColor(const QColor &color)
{
    this->prompt_line_color = color;
}

void SidebarOptionsButton::setDrawPromptLineEnable(bool enable)
{
    this->is_draw_prompt_line = enable;
}

void SidebarOptionsButton::setFilletRadius(int radius)
{
    this->fillet_radius = radius;
}

int SidebarOptionsButton::index()
{
    return this->index_;
}

void SidebarOptionsButton::setIndex(int index)
{
    this->index_ = index;
}

void SidebarOptionsButton::setIcon(const QPixmap &icon)
{
    this->icon_label->setPixmap(icon);
}

void SidebarOptionsButton::setText(const QString &text)
{
    this->text_label->setText(text);
}

bool SidebarOptionsButton::hitButton(const QPoint &) const
{
    return true;
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
void SidebarOptionsButton::enterEvent(QEnterEvent *event)
{
    QRadioButton::enterEvent(event);
    if (this->isChecked())
        this->background_brush.setColor(this->clicked_enter_color);
    else
        this->background_brush.setColor(this->dis_clicked_enter_color);
    this->update();
}
#else
void SidebarOptionsButton::enterEvent(QEvent *event)
{
    QRadioButton::enterEvent(event);
    if (this->isChecked())
        this->background_brush.setColor(this->clicked_enter_color);
    else
        this->background_brush.setColor(this->dis_clicked_enter_color);
    this->update();
}
#endif

void SidebarOptionsButton::leaveEvent(QEvent *event)
{
    QRadioButton::leaveEvent(event);
    if (this->isChecked())
        this->background_brush.setColor(this->clicked_color);
    else
        this->background_brush.setColor(this->dis_clicked_Color);
    this->update();
}

void SidebarOptionsButton::mousePressEvent(QMouseEvent *event)
{
    QRadioButton::mousePressEvent(event);
    this->background_brush.setColor(this->pressColor);
    this->update();
}

void SidebarOptionsButton::mouseReleaseEvent(QMouseEvent *event)
{
    QRadioButton::mouseReleaseEvent(event);
    this->background_brush.setColor(this->clicked_color);
    this->update();
}

void SidebarOptionsButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing); // 抗锯齿

    // 绘制背景色
    painter.setBrush(this->background_brush);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.drawRoundedRect(rect(), this->fillet_radius, this->fillet_radius);

    // 绘制选中提示线条
    if (this->isChecked() && this->is_draw_prompt_line)
    {
        QPen pen(this->prompt_line_color);
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);

        int lineWidth = 3;
        pen.setWidth(lineWidth);

        painter.setPen(pen);

        int x = lineWidth;
        int y1 = this->height() / 4;
        int y2 = y1 * 2 + y1;

        painter.drawLine(x, y1, x, y2);
    }
}
