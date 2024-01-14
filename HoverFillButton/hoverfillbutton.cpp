#include "hoverfillbutton.h"

HoverFillButton::HoverFillButton(QWidget *parent, AnimationType animationType)
    : HoverFillButtonBase::HoverFillButtonBase(parent), animation_type{animationType}
{
    initializeMemberVariable();
}
HoverFillButton::HoverFillButton(const QString &text, QWidget *parent, AnimationType animationType)
    : HoverFillButtonBase::HoverFillButtonBase(text, parent), animation_type{animationType}
{
    initializeMemberVariable();
}

HoverFillButton::HoverFillButton(const QIcon &icon, const QString &text, QWidget *parent, AnimationType animationType)
    : HoverFillButtonBase::HoverFillButtonBase(icon, text, parent), animation_type{animationType}
{
    initializeMemberVariable();
}

void HoverFillButton::setAnimationType(AnimationType animationType)
{
    animation_type = animationType;
    initializeMemberVariable();
}

void HoverFillButton::paintValueReduction()
{
    HoverFillButtonBase::paintValueReduction();
    update();
}

void HoverFillButton::paintValueIncrease()
{
    HoverFillButtonBase::paintValueIncrease();
    update();
}

void HoverFillButton::enterEvent(QEnterEvent *event)
{
    HoverFillButtonBase::enterEvent(event);

    connect(timer, &QTimer::timeout, this, &HoverFillButton::paintValueIncrease); // 定时器触发，半径增大
    switch (animation_type)
    {
    case AnimationType::CircularFill:    // 进入点圆形填充
        mouse_point = event->position(); // 记录鼠标进入坐标
        break;
    default:
        break;
    }
    timer->start();
}

void HoverFillButton::leaveEvent(QEvent *event)
{
    HoverFillButtonBase::leaveEvent(event);

    connect(timer, &QTimer::timeout, this, &HoverFillButton::paintValueReduction);
    switch (animation_type)
    {
    case AnimationType::CircularFill: // 进入点圆形填充
        mouse_point = mapFromGlobal(QCursor::pos());
        break;
    default:
        break;
    }
    timer->start();
}

void HoverFillButton::resizeEvent(QResizeEvent *event)
{
    HoverFillButtonBase::resizeEvent(event);
    initializeMemberVariable();
}

void HoverFillButton::paintEvent(QPaintEvent *event)
{
    HoverFillButtonBase::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing); // 抗锯齿和文字走样
    painter.setBrush(fill_brush);                                        // 设置填充颜色
    painter.setPen(Qt::NoPen);                                                   // 设置无边框
    painter.setClipPath(path);                                                   // 绘制设置路径
    painter.setFont(font());                                                     // 设置字体
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
    if (paint_value > 0)
    {
        painter.setPen(hover_text_color);                  // 设置画笔颜色
        painter.drawText(rect(), Qt::AlignCenter, text()); // 绘制文字
    }
}

void HoverFillButton::initializeMemberVariable()
{
    switch (animation_type)
    {
    case AnimationType::CircularFill:                                      // 进入点,圆形填充
        max_painte_value = qSqrt(width() * width() + height() * height()); // 计算最大半径
        break;
    case AnimationType::CrossFill: // 左右两个小圆交叉填充
        max_painte_value = qPow(qPow(width(), 2) + qPow(height(), 2), 0.5);
        break;
    case AnimationType::DiagonalRectangle: // 斜着的矩形填充
        max_painte_value = qPow(qPow(width(), 2) + qPow(height(), 2), 0.5);
        break;
    case AnimationType::BottomCircle: // 底部圆形填充
        max_painte_value = qPow(qPow(width(), 2) + qPow(height(), 2), 0.5);
        break;
    case AnimationType::LeftRectangle: // 左侧矩形填充
        max_painte_value = qPow(qPow(width(), 2) + qPow(height(), 2), 0.5);
        break;
    }
}
