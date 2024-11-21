#include "FancyButton.h"
#include "FancyStyle.h"

PushButton::PushButton(QWidget *parent)
    :QPushButton(parent)
{
    this->setStyle(new PushButtonStyleBase(this));
}

PushButton::PushButton(const QString &text, QWidget *parent)
    :QPushButton(text,parent)
{
    this->setStyle(new PushButtonStyleBase(this));
}

PushButton::PushButton(const QIcon &icon, const QString &text, QWidget *parent)
    :QPushButton(icon,text,parent)
{
    this->setStyle(new PushButtonStyleBase(this));
}

PushButton::PushButton(const Icon &icon, const QString &text, QWidget *parent):
    PushButton(text,parent)
{
    this->setIcon(icon);
}

PushButton::PushButton(FancyIcon iconEnum, const QString &text, QWidget *parent):
    PushButton(Icon(iconEnum),text,parent)
{
}

void PushButton::setCheckable(bool checkable)
{
    ((PushButtonStyleBase*)this->style())->setCheckable(checkable);
    QPushButton::setCheckable(checkable);
}
void PushButton::setIcon(const Icon &icon)
{
    QPushButton::setIcon(icon);
}

void PushButton::setIcon(FancyIcon iconEnum)
{
    QPushButton::setIcon(Icon(iconEnum));
}

ThemeButton::ThemeButton(QWidget *parent)
    :QPushButton(parent)
{
    this->setStyle(new ThemePushButtonSyle(this));
}

ThemeButton::ThemeButton(const QString &text, QWidget *parent)
    :QPushButton(text,parent)
{
    this->setStyle(new ThemePushButtonSyle(this));
}

ThemeButton::ThemeButton(const QIcon &icon, const QString &text, QWidget *parent)
    :QPushButton(icon,text,parent)
{
    this->setStyle(new ThemePushButtonSyle(this));
}

ThemeButton::ThemeButton(const Icon &icon, const QString &text, QWidget *parent)
    :ThemeButton(text,parent)
{
    this->setIcon(icon);
}

ThemeButton::ThemeButton(FancyIcon iconEnum, const QString &text, QWidget *parent)
    :ThemeButton(text,parent)
{
    this->setIcon(Icon(iconEnum));
}

void ThemeButton::setIcon(const Icon &icon)
{
    icon.setColorChangeBaseOnThemeColor(true);
    QPushButton::setIcon(icon);
}

void ThemeButton::setIcon(FancyIcon iconEnum)
{
    this->setIcon(Icon(iconEnum));
}


RippleButton::RippleButton(QWidget *parent)
    :QPushButton(parent)
{
    this->setStyle(new RipplePushButtonStyle(this));
}

RippleButton::RippleButton(const QString &text, QWidget *parent)
    :QPushButton(text,parent)
{
    this->setStyle(new RipplePushButtonStyle(this));
}

RippleButton::RippleButton(const QIcon &icon, const QString &text, QWidget *parent)
    :QPushButton(icon,text,parent)
{
    this->setStyle(new RipplePushButtonStyle(this));
}

RippleButton::RippleButton(const Icon &icon, const QString &text, QWidget *parent):
    RippleButton(text,parent)
{
    this->setIcon(icon);
}

RippleButton::RippleButton(FancyIcon iconEnum, const QString &text, QWidget *parent):
    RippleButton(text,parent)
{
    this->setIcon(Icon(iconEnum));
}

void RippleButton::setIcon(const Icon &icon)
{
    icon.setColorChangeBaseOnThemeColor(true);
    QPushButton::setIcon(icon);
}

void RippleButton::setIcon(FancyIcon iconEnum)
{
    this->setIcon(Icon(iconEnum));
}

TransparentButton::TransparentButton(QWidget *parent)
    :QPushButton(parent)
{
    this->setStyle(new TransparentPushButtonStyle(this));
}

TransparentButton::TransparentButton(const QString &text, QWidget *parent)
    :QPushButton(text,parent)
{
    this->setStyle(new TransparentPushButtonStyle(this));
}

TransparentButton::TransparentButton(const QIcon &icon, const QString &text, QWidget *parent)
    :QPushButton(icon,text,parent)
{
    this->setStyle(new TransparentPushButtonStyle(this));
}

TransparentButton::TransparentButton(const Icon &icon, const QString &text, QWidget *parent):
    TransparentButton(text,parent)
{
    this->setIcon(icon);
}

TransparentButton::TransparentButton(FancyIcon iconEnum, const QString &text, QWidget *parent):
    TransparentButton(Icon(iconEnum),text,parent)
{
}

void TransparentButton::setCheckable(bool checkable)
{
    ((TransparentPushButtonStyle*)this->style())->setCheckable(checkable);
    QPushButton::setCheckable(checkable);
}

void TransparentButton::preserveTransparency(bool preserve)
{
    ((TransparentPushButtonStyle*)this->style())->preserveTransparency(preserve);
}

void TransparentButton::setIcon(const Icon &icon)
{
    QPushButton::setIcon(icon);
}

void TransparentButton::setIcon(FancyIcon iconEnum)
{
    QPushButton::setIcon(Icon(iconEnum));
}


ToolButtonBase::ToolButtonBase(QWidget *parent):
    QToolButton(parent)
{
    this->setStyle(new ToolButtonStyleBase(this));
}

void ToolButtonBase::setCheckable(bool checkable)
{
    ((ToolButtonStyleBase*)(this->style()))->setCheckable(true);
    QToolButton::setCheckable(true);
}

void ToolButtonBase::setIcon(const Icon &icon)
{
    QToolButton::setIcon(icon);
}

void ToolButtonBase::setIcon(FancyIcon iconEnum)
{
    QToolButton::setIcon(Icon(iconEnum));
}

FillAnimationButton::FillAnimationButton(QWidget *parent, AnimationType animationType) :
    QPushButton::QPushButton(parent)
{
    initializeMemberVariable(animationType);
}
FillAnimationButton::FillAnimationButton(const QString &text, QWidget *parent, AnimationType animationType) :
    QPushButton::QPushButton(text, parent)
{
    initializeMemberVariable(animationType);
}

FillAnimationButton::FillAnimationButton(const QIcon &icon, const QString &text, QWidget *parent, AnimationType animationType) :
    QPushButton::QPushButton(icon, text, parent)
{
    initializeMemberVariable(animationType);
}

void FillAnimationButton::setAnimationType(AnimationType animationType)
{
    _animationType = animationType;
    calculatedMaximumRadius();
}

void FillAnimationButton::setFillTime(int millisecond)
{
    _animation->setDuration(millisecond);
}

void FillAnimationButton::setFillBrush(const QBrush &fillBrush)
{
    _fillBrush = fillBrush;
}

void FillAnimationButton::setRadius(int radius)
{
    this->radius = radius;
    _path.clear();
    _path.addRoundedRect(rect(), radius, radius);
    update();
}

void FillAnimationButton::setTextColor(const QColor &normalTextColor, const QColor &hoverTextColor)
{
    _normalTextColor = normalTextColor;
    _hoverTextColor = hoverTextColor;
}

void FillAnimationButton::addShadow(int right, int down, qreal blurRadius, const QColor &shadowColor)
{
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(right, down);    // 阴影的偏移量（右，下）
    shadow->setColor(shadowColor);     // 阴影的颜色
    shadow->setBlurRadius(blurRadius); // 控制阴影的模糊程度（光源距离）
    this->setGraphicsEffect(shadow);
}

void FillAnimationButton::setBackgroundBrush(const QBrush &brush)
{
    _backgroundBrush = brush;
    update();
};

void FillAnimationButton::enterEvent(QEnterEvent *event)
{
    QPushButton::enterEvent(event);
    _isCursorInside = true;
    switch (_animationType)
    {
    case AnimationType::CircularFill:    // 进入点圆形填充
        _mousePoint = event->position(); // 记录鼠标进入坐标
        break;
    default:
        break;
    }
    _animation->setDirection(QAbstractAnimation::Direction::Forward);
    _animation->start();
}

void FillAnimationButton::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    _isCursorInside = false;
    switch (_animationType)
    {
    case AnimationType::CircularFill: // 进入点圆形填充
        _mousePoint = mapFromGlobal(QCursor::pos());
        break;
    default:
        break;
    }
    _animation->setDirection(QAbstractAnimation::Direction::Backward);
    _animation->start();
}

void FillAnimationButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    _path.clear();
    _path.addRoundedRect(rect(), radius, radius);
    calculatedMaximumRadius();
}

void FillAnimationButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);    // 抗锯齿和防文字走样
    painter.fillPath(_path, _backgroundBrush);                                      // 绘制背景
    painter.setClipPath(_path);                                                     // 绘制设置路径
    painter.setBrush(_fillBrush);                                                   // 设置填充颜色
    painter.setPen(Qt::NoPen);
    switch (_animationType)
    {
    case AnimationType::CircularFill: // 进入点圆形填充
        if (!_mousePoint.isNull())
            painter.drawEllipse(_mousePoint, _paintValue, _paintValue); // 从鼠标进入点开始画圆
        break;
    case AnimationType::CrossFill: // 左右两个小圆交叉填充
        painter.drawEllipse(QPoint(0, 0), _paintValue, _paintValue);
        painter.drawEllipse(QPoint(width(), height()), _paintValue, _paintValue);
        break;
    case AnimationType::DiagonalRectangle:                                              // 斜着的矩形填充
        painter.save();                                                                 // 保存状态
        painter.translate(width() / 2, height() / 2);                                   // 将坐标原点移动到按钮中心
        painter.rotate(45);                                                             // 将坐标系旋转45度
        painter.drawRect(QRectF(-_paintValue / 2, -width() / 2, _paintValue, width())); // 绘制矩形
        painter.restore();                                                              // 恢复状态
        break;
    case AnimationType::BottomCircle: // 底部圆形填充
        painter.save();
        painter.translate(width() / 2, int(height() * 1.5));
        painter.drawEllipse(QPoint(0, 0), _paintValue, _paintValue);
        painter.restore();
        break;
    case AnimationType::LeftRectangle: // 左侧矩形填充
        painter.save();
        if (_isCursorInside) // 光标在内部,从左到右绘制矩形
            painter.drawRect(QRectF(0, 0, _paintValue, height()));
        else // 坐标系翻转,从右到左绘画矩形
        {
            painter.translate(width(), height());
            painter.rotate(180);
            painter.drawRect(QRectF(0, 0, _paintValue, height()));
        }
        painter.restore();
        break;
    }
    painter.setFont(font());                                                            // 设置字体
    if (_paintValue > 0)
        painter.setPen(_hoverTextColor);
    else
        painter.setPen(_normalTextColor);
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

void FillAnimationButton::initializeMemberVariable(AnimationType animationType)
{
    _animationType = animationType;
    _animation = new QVariantAnimation(this);
    _animation->setStartValue(0);
    _animation->setDuration(300);
    connect(_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value){this->_paintValue = value.value<int>();update();});
    _path.addRoundedRect(rect(), radius, radius);
}

void FillAnimationButton::calculatedMaximumRadius()
{
    int max_painte_value; // 最大绘制值
    switch (_animationType)
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
    _animation->setEndValue(max_painte_value);
}
