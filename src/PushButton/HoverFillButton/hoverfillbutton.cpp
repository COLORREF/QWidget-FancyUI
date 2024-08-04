#include "hoverfillbutton.h"

HoverFillButton::HoverFillButton(QWidget *parent, AnimationType animationType) :
    QPushButton::QPushButton(parent)
{
    initializeMemberVariable(animationType);
}
HoverFillButton::HoverFillButton(const QString &text, QWidget *parent, AnimationType animationType) :
    QPushButton::QPushButton(text, parent)
{
    initializeMemberVariable(animationType);
}

HoverFillButton::HoverFillButton(const QIcon &icon, const QString &text, QWidget *parent, AnimationType animationType) :
    QPushButton::QPushButton(icon, text, parent)
{
    initializeMemberVariable(animationType);
}

void HoverFillButton::setAnimationType(AnimationType animationType)
{
    animation_type = animationType;
    calculatedMaximumRadius();
}

void HoverFillButton::setFillTime(int millisecond)
{
    animation->setDuration(millisecond);
}

void HoverFillButton::setFillBrush(const QBrush &fillBrush)
{
    fill_brush = fillBrush;
}

void HoverFillButton::setRadius(int radius)
{
    this->radius = radius;
    path.clear();
    path.addRoundedRect(rect(), radius, radius);
    update();
}

void HoverFillButton::setTextColor(const QColor &normalTextColor, const QColor &hoverTextColor)
{
    hover_text_color = hoverTextColor;
    normal_text_color = normalTextColor;
}

void HoverFillButton::addShadow(int right, int down, qreal blurRadius, const QColor &shadowColor)
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(right, down);    // 阴影的偏移量（右，下）
    shadow->setColor(shadowColor);     // 阴影的颜色
    shadow->setBlurRadius(blurRadius); // 控制阴影的模糊程度（光源距离）
    this->setGraphicsEffect(shadow);
}

void HoverFillButton::setBackgroundBrush(const QBrush &brush)
{
    background_brush = brush;
    update();
};

void HoverFillButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    is_cursor_inside = true;
    switch (animation_type)
    {
    case AnimationType::CircularFill:    // 进入点圆形填充
        mouse_point = event->position(); // 记录鼠标进入坐标
        break;
    default:
        break;
    }
    animation->setDirection(QAbstractAnimation::Direction::Forward);
    animation->start();
}

void HoverFillButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    is_cursor_inside = false;
    switch (animation_type)
    {
    case AnimationType::CircularFill: // 进入点圆形填充
        mouse_point = mapFromGlobal(QCursor::pos());
        break;
    default:
        break;
    }
    animation->setDirection(QAbstractAnimation::Direction::Backward);
    animation->start();
}

void HoverFillButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    path.clear();
    path.addRoundedRect(rect(), radius, radius);
    calculatedMaximumRadius();
}

void HoverFillButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // 抗锯齿和防文字走样
    painter.fillPath(path, background_brush);                                    // 绘制背景
    painter.setClipPath(path);                                                   // 绘制设置路径
    painter.setBrush(fill_brush);                                                // 设置填充颜色
    painter.setPen(Qt::NoPen);
    switch (animation_type)
    {
    case AnimationType::CircularFill: // 进入点圆形填充
        if (!mouse_point.isNull())
            painter.drawEllipse(mouse_point, paint_value, paint_value); // 从鼠标进入点开始画圆
        break;
    case AnimationType::CrossFill: // 左右两个小圆交叉填充
        painter.drawEllipse(QPoint(0, 0), paint_value, paint_value);
        painter.drawEllipse(QPoint(width(), height()), paint_value, paint_value);
        break;
    case AnimationType::DiagonalRectangle:                                              // 斜着的矩形填充
        painter.save();                                                                 // 保存状态
        painter.translate(width() / 2, height() / 2);                                   // 将坐标原点移动到按钮中心
        painter.rotate(45);                                                             // 将坐标系旋转45度
        painter.drawRect(QRectF(-paint_value / 2, -width() / 2, paint_value, width())); // 绘制矩形
        painter.restore();                                                              // 恢复状态
        break;
    case AnimationType::BottomCircle: // 底部圆形填充
        painter.save();
        painter.translate(width() / 2, int(height() * 1.5));
        painter.drawEllipse(QPoint(0, 0), paint_value, paint_value);
        painter.restore();
        break;
    case AnimationType::LeftRectangle: // 左侧矩形填充
        painter.save();
        if (is_cursor_inside) // 光标在内部,从左到右绘制矩形
            painter.drawRect(QRectF(0, 0, paint_value, height()));
        else // 坐标系翻转,从右到左绘画矩形
        {
            painter.translate(width(), height());
            painter.rotate(180);
            painter.drawRect(QRectF(0, 0, paint_value, height()));
        }
        painter.restore();
        break;
    }
    painter.setFont(font());                                                     // 设置字体
    if (paint_value > 0)
        painter.setPen(hover_text_color);
    else
        painter.setPen(normal_text_color);
    QIcon icon = this->icon();
    if(!icon.isNull())
    {
        QSize size = iconSize();
        int iocn_width = size.width();
        int icon_height = size.height();
        QString text = this->text();
        QRect text_rect = painter.fontMetrics().boundingRect(text);
        int text_width = text_rect.width();
        int text_height = text_rect.height();
        int x_i = (width() - (iocn_width + text_width))/2;
        int y_i = (height()- qMax(icon_height,text_height))/2;
        painter.drawPixmap(QRect(QPoint(x_i,y_i),size),icon.pixmap(size));
        painter.drawText(QRect(x_i + iocn_width+4, y_i, text_width + 4, text_height), Qt::AlignCenter, text);// 绘制文字
    }
    else
        painter.drawText(rect(), Qt::AlignCenter, text());
}

void HoverFillButton::initializeMemberVariable(AnimationType animationType)
{
    animation_type = animationType;
    animation = new QVariantAnimation(this);
    animation->setStartValue(0);
    animation->setDuration(300);
    connect(animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value){this->paint_value = value.value<int>();update();});
    path.addRoundedRect(rect(), radius, radius);
}

void HoverFillButton::calculatedMaximumRadius()
{
    int max_painte_value; // 最大绘制值
    switch (animation_type)
    {
    case AnimationType::CircularFill:                                      // 进入点,圆形填充
        max_painte_value = qSqrt(width() * width() + height() * height()); // 计算最大半径
        break;
    case AnimationType::CrossFill:         // 左右两个小圆交叉填充
        [[fallthrough]];
    case AnimationType::DiagonalRectangle: // 斜着的矩形填充
        [[fallthrough]];
    case AnimationType::BottomCircle:      // 底部圆形填充
        [[fallthrough]];
    case AnimationType::LeftRectangle:     // 左侧矩形填充
        max_painte_value = qPow(qPow(width(), 2) + qPow(height(), 2), 0.5);
        break;
    }
    animation->setEndValue(max_painte_value);
}
