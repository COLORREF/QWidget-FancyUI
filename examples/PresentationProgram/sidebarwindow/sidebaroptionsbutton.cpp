#include "sidebaroptionsbutton.h"

SidebarOptionsButton::SidebarOptionsButton(QWidget* parent,int index, int minimumWidth, int fixedHeight):
    QRadioButton{parent},
    icon_label{new QLabel(this)},
    text_label{new QLabel(this)},
    index{index}
{
    QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setMinimumWidth(minimumWidth);
    this->setFixedHeight(fixedHeight);
    this->resize(minimumWidth,fixedHeight);

    this->icon_label->resize(size()/2);
    this->icon_label->move(width()/4,height()/4);
    this->icon_label->setScaledContents(true);

    this->text_label->move(width()+parent->layout()->contentsMargins().right(),height()/4);

    connect(this, &SidebarOptionsButton::toggled,this,[&](bool isClicked){
        if(isClicked)
        {
            emit selectedIndex(this->index);
            this->background_brush.setColor(clicked_color);
        }
        else
            this->background_brush.setColor(dis_clicked_Color);
        update();
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
    this->prompt_line_clolor = color;
}

void SidebarOptionsButton::setDrawPromptLineEnable(bool enable)
{
    this->is_draw_prompt_line = enable;
}

void SidebarOptionsButton::setFilletRadius(int radius)
{
    this->fillet_radius = radius;
}

void SidebarOptionsButton::setIndex(int index)
{
    this->index = index;
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

void SidebarOptionsButton::enterEvent(QEnterEvent *event)
{
    QRadioButton::enterEvent(event);
    if(this->isChecked())
        background_brush.setColor(clicked_enter_color);
    else
        background_brush.setColor(dis_clicked_enter_color);
    update();
}

void SidebarOptionsButton::leaveEvent(QEvent *event)
{
    QRadioButton::leaveEvent(event);
    if(this->isChecked())
        background_brush.setColor(clicked_color);
    else
        background_brush.setColor(dis_clicked_Color);
    update();
}

void SidebarOptionsButton::mousePressEvent(QMouseEvent *event)
{
    QRadioButton::mousePressEvent(event);
    background_brush.setColor(pressColor);
    update();
}

void SidebarOptionsButton::mouseReleaseEvent(QMouseEvent *event)
{
    QRadioButton::mouseReleaseEvent(event);
    background_brush.setColor(clicked_color);
    update();
}

void SidebarOptionsButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing); // 抗锯齿

    //绘制背景色
    painter.setBrush(background_brush);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.drawRoundedRect(rect(),fillet_radius,fillet_radius);

    //绘制选中提示线条
    if(this->isChecked() && this->is_draw_prompt_line)
    {
        QPen pen(prompt_line_clolor);
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);

        int lineWidth = 3;
        pen.setWidth(lineWidth);

        painter.setPen(pen);

        int x = lineWidth;
        int y1 =this->height()/4;
        int y2 = y1*2+y1;

        painter.drawLine(x, y1, x, y2);
    }
}

