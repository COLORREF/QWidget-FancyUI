#include "FancyStyle.h"

ControlColors::ControlColors():
    QObject(nullptr),
    _disEnabled(180,180,180),
    _normalBorder(131,131,131),
    _buttonBorder(236,236,236),
    _buttonBottomLine(211,211,211),
    _buttonBackground(252, 252, 252),
    _prominence(212,78,125),
    _text(Qt::GlobalColor::black),
    _theme(_prominence)
{
    connect(this,&ControlColors::prominenceColorChange,this,&ControlColors::setTheme);
    connect(Theme::themeObject(),&Theme::themeChange,this,[this](){
        if(Theme::isLigth())
        {
            this->setNormalBorder(QColor(160,160,160));
            this->setButtonBorder(QColor(236,236,236));
            this->setButtonBottomLine(QColor(211,211,211));
            this->setButtonBackground(QColor(252, 252, 252));
            this->setTheme(this->_prominence);
            this->setText(Qt::GlobalColor::black);
        }
        else
        {
            this->setNormalBorder( QColor(131,131,131));
            this->setButtonBorder(QColor(53,53,53));
            this->setButtonBottomLine(QColor(44,44,44));
            this->setButtonBackground(QColor(55, 55,55));
            this->setTheme(this->_prominence.lighter(110));
            this->setText(Qt::GlobalColor::white);
        }
    });
};

ControlColors * ControlColors::controlColors()
{
    static ControlColors colors;
    return &colors;
};

QColor ControlColors::hover(const QColor &color)
{
    if(isColorLight(color))
    {
        if(isColorCloseToWhite(color))
            return color.darker(103);
        return color.darker(110);
    }
    return color.lighter(110);
}

QColor ControlColors::autoTextColor(const QColor &backgroundColor)
{
    if(isColorCloseToWhite(backgroundColor,165))
        return Qt::black;
    return Qt::white;
}

QColor ControlColors::transparentHover()
{
    if(Theme::isLigth())
        return this->transparentHoverLight();
    else
        return this->transparentHoverDark();
}

QColor ControlColors::transparentClick()
{
    if(Theme::isLigth())
        return this->transparentClickLight();
    else
        return this->transparentClickDark();
}


FancyStyleBase::FancyStyleBase(QObject *parent):
    QProxyStyle(nullptr)
{
    this->setParent(parent);
}

void FancyStyleBase::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    if(enabled)
        const_cast<QPalette &>(pal).setColor(textRole, this->_colors->text());
    else
        const_cast<QPalette &>(pal).setColor(textRole, this->_colors->disEnabled());
    QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
}

ControlColors* FancyStyleBase::_colors = ControlColors::controlColors();

constexpr int RADIO_TIME = 150;
constexpr qreal RADIO_FILL_CIRCLE_RADIUS = 5.0;
constexpr qreal RADIO_HALF_INDICATOR = 8.0;

constexpr int CHECKBOX_TIME = 200;
constexpr int CHECKBOX_RADIUS = 12;
constexpr qreal CHECKBOX_SIDE = 16;

constexpr int RIPPLE_BUTTON_LIGHTER_RATIO = 150;
constexpr int RIPPLE_BUTTON_TIME = 700;

RadioButtonStyle::RadioButtonStyle(QRadioButton *parent):
    FancyStyleBase(parent),
    _group(new QParallelAnimationGroup(parent)),
    _jumpAnimation(new SimpleAnimation(RADIO_HALF_INDICATOR,RADIO_HALF_INDICATOR,RADIO_TIME+50,true,parent)),
    _indicatorAnimation(new SimpleAnimation(0.0,RADIO_FILL_CIRCLE_RADIUS,RADIO_TIME,true,parent))
{
    this->_indicatorAnimation->setUpdate(parent);
    this->_jumpAnimation->setKeyValueAt(0.5,0.0);
    this->_jumpAnimation->setUpdate(parent);
    this->_jumpAnimation->_runTimeValue = RADIO_HALF_INDICATOR;
    this->_group->addAnimation(this->_indicatorAnimation);
    connect(parent,&QRadioButton::toggled,this,[this](bool checked){
        this->_group->setDirection((QAbstractAnimation::Direction)!checked);
        this->_group->start();
    });
    connect(Theme::themeObject(),&Theme::themeChange,this,[parent](){parent->update();});
}

void RadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_IndicatorRadioButton)
    {
        CheckableControlState s(option->state);
        if(s.unenable_select)_UnEnableOn = true;
        if(s.normal)_UnEnableOn = false;
        bool unenable_last_select = !s.enable && s.off && _UnEnableOn;

        quint8 state = s.flage;
        if(unenable_last_select)state = 8;// 禁用未选中，但上一次是禁用且选中

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        QRectF indicator(this->subElementRect(SE_RadioButtonIndicator, option, widget));
        indicator.translate(2.0,0);
        QPointF center = indicator.center();
        indicator.adjust(0.5, 0.5, -0.5, -0.5);//画笔宽度修正

        // 设置画笔，绘制外圆
        QPen pen;
        switch(state)
        {
        case 0:
            pen.setColor(this->_colors->normalBorder());
            break;
        case 1:[[fallthrough]];
        case 2:[[fallthrough]];
        case 3:
            pen.setColor(this->_colors->theme());
            break;
        case 4:
            pen.setColor(this->_colors->theme());
            pen.setWidth(2);
            indicator.adjust(0.5, 0.5, -0.5, -0.5); //画笔宽度修正
            break;
        case 5:
            pen.setColor(this->_colors->theme());
            break;
        case 6:[[fallthrough]];
        case 7:[[fallthrough]];
        case 8:
            pen.setColor(this->_colors->disEnabled());
            break;
        }
        painter->setPen(pen);

        qreal r;
        if(_enableJumpAnimation)
            r = this->_jumpAnimation->_runTimeValue.toReal();
        else
            r = RADIO_HALF_INDICATOR;
        painter->drawEllipse(center,r,r);


        // 根据状态绘制内圆
        if(state < 4)
            painter->setBrush(this->_colors->theme());
        else
            painter->setBrush(this->_colors->disEnabled());
        painter->setPen(Qt::PenStyle::NoPen);
        r = this->_indicatorAnimation->_runTimeValue.toReal();
        if(state > 6)
            r = RADIO_FILL_CIRCLE_RADIUS;
        if(state == 3)
            painter->drawEllipse(center, r + 1.0, r + 1.0);
        else
            painter->drawEllipse(center, r, r);
        painter->restore();
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter,widget);
}

void RadioButtonStyle::enableJumpAnimation(bool isEnable)
{
    if (isEnable != this->_enableJumpAnimation)
    {
        this->_enableJumpAnimation = isEnable;
        if (isEnable)
            this->_group->addAnimation(this->_jumpAnimation);
        else
            this->_group->removeAnimation(this->_jumpAnimation);
    }
}

CheckBoxStyle::CheckBoxStyle(QCheckBox *parent):
    FancyStyleBase(parent),
    _animation(new SimpleAnimation(0,CHECKBOX_RADIUS,CHECKBOX_TIME,false,parent))
{
    this->_animation->setUpdate(parent);
    connect(parent,&QCheckBox::toggled,this->_animation,&SimpleAnimation::reverseDirectionAndStart);
    connect(Theme::themeObject(),&Theme::themeChange,this,[parent](){parent->update();});
}


void CheckBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_IndicatorCheckBox)
    {
        QRectF indicatorRect(QProxyStyle::subElementRect(SE_CheckBoxIndicator, option, widget));
        indicatorRect.translate(1.5,0);
        qreal dx = (CHECKBOX_SIDE-indicatorRect.width())/2;
        qreal dy = (CHECKBOX_SIDE-indicatorRect.height())/2;
        indicatorRect.adjust(-dx,-dy,dx,dy);
        indicatorRect.adjust(0.5,0.5,-0.5,-0.5);//画笔宽度修正
        QPointF center = indicatorRect.center();
        CheckableControlState s(option->state);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        // 设置画笔，绘制外矩形
        QPen pen;
        switch(s.flage)
        {
        case 0:
            pen.setColor(this->_colors->normalBorder());
            break;
        case 1:[[fallthrough]];
        case 3:[[fallthrough]];
        case 5:[[fallthrough]];
        case 2:
            pen.setColor(this->_colors->theme());
            break;
        case 4:
            pen.setColor(this->_colors->theme());
            pen.setWidth(2);
            indicatorRect.adjust(0.5,0.5,-0.5,-0.5);//画笔宽度修正
            break;
        case 6:[[fallthrough]];
        case 7:[[fallthrough]];
        case 8:
            pen.setColor(this->_colors->disEnabled());
            break;
        }
        painter->setPen(pen);
        painter->drawRoundedRect(indicatorRect,3,3);


        // 设置画刷，根据状态绘制内矩形
        if( s.flage < 6)
            painter->setBrush(this->_colors->theme());
        else if(s.flage > 5)
            painter->setBrush(this->_colors->disEnabled());
        painter->setPen(Qt::NoPen);

        QPainterPath clipPath;// 圆角矩形裁剪路径
        clipPath.addRoundedRect(indicatorRect, 3, 3);
        painter->setClipPath(clipPath);
        int r = _animation->_runTimeValue.toInt();
        painter->drawEllipse(center,r,r);

        // 绘制勾
        if(s.on)
        {
            int x = indicatorRect.x();
            int y = indicatorRect.y();
            int l = indicatorRect.width();
            QPointF p1(0.23875*l+x,0.58125*l+y);
            QPointF p2(0.42625*l+x,0.8*l+y);
            QPointF p3(0.86375*l+x,0.26875*l+y);
            pen.setColor(Qt::GlobalColor::white);
            pen.setWidthF(1.5);
            if(s.over)
                pen.setWidth(2);
            if(s.flage == 2)
                pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(p1,p2);
            painter->drawLine(p2,p3);
        }
        painter->restore();
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter,widget);
}

PushButtonStyleBase::PushButtonStyleBase(QAbstractButton *parent, bool checkable):
    FancyStyleBase(parent),
    _checkable(checkable)
{
    connect(Theme::themeObject(),&Theme::themeChange,this,[parent](){parent->update();});
}

void PushButtonStyleBase::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    // Windows10 和 Windows11 相同的代码运行结果不一样
    // 下面的写法可以使得效果一致，但不确定有没有其他问题
    QAbstractButton* parent = dynamic_cast<QAbstractButton*>(this->parent());
    if(parent)
    {
        if(enabled)
        {
            if(parent->isChecked())
            {
                QColor textColor = this->_colors->autoTextColor(_colors->theme());
                painter->setPen(textColor);
                const_cast<QPalette &>(pal).setColor(textRole, textColor);
            }
            else
            {
                const QColor& textColor = this->_colors->text();
                painter->setPen(textColor);
                const_cast<QPalette &>(pal).setColor(textRole, textColor);
            }
        }
        else
        {
            QColor textColor = this->_colors->disEnabled().lighter(130);
            painter->setPen(textColor);
            const_cast<QPalette &>(pal).setColor(textRole, textColor);
        }
    }
    QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
}

void PushButtonStyleBase::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element == QStyle::CE_PushButtonBevel)
    {
        QRect rect = QProxyStyle::subElementRect(QStyle::SE_PushButtonFocusRect, option, widget);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        QPainterPath clip_path;// 圆角矩形裁剪路径
        clip_path.addRoundedRect(rect,_buttonRadius,_buttonRadius);
        if(this->_checkable)
            this->checkablePaint(painter,CheckableWidgetState(option->state).flage,rect,clip_path);
        else
            this->unCheckablePaint(painter,UnCheckableControlState(option->state).flage,rect,clip_path);
        painter->restore();
        return;
    }
    QProxyStyle::drawControl(element,option,painter,widget);
}

void PushButtonStyleBase::setCheckable(bool checkable)
{
    this->_checkable = checkable;
}

QLine PushButtonStyleBase::bottomLine(const QRect &rect, int filletRadius)const
{
    return QLine(rect.x()+filletRadius,rect.y()+rect.height(),rect.x()+rect.width()-filletRadius,rect.y()+rect.height());
}

void PushButtonStyleBase::unCheckablePaint(QPainter *painter, qint8 state,const QRect& rect, const QPainterPath& clipPath) const
{
    switch (state)
    {
    case 0: [[fallthrough]];
    case 1:
        painter->setBrush(this->_colors->buttonBackground());
        break;
    case 2:
        painter->setBrush(this->_colors->hover(this->_colors->buttonBackground()));
        break;
    case 3:
        painter->setBrush(this->_colors->disEnabled());
        break;
    }
    QPen pen(this->_colors->buttonBorder());
    painter->setPen(pen);
    painter->drawPath(clipPath);
    pen.setColor(this->_colors->buttonBottomLine());
    painter->drawLine(this->bottomLine(rect,_buttonRadius));
}

void PushButtonStyleBase::checkablePaint(QPainter *painter, qint8 state, const QRect &rect, const QPainterPath &clipPath) const
{
    switch(state)
    {
    case 0:
        painter->setBrush(this->_colors->buttonBackground());
        break;
    case 1:[[fallthrough]];
    case 2:
        painter->setBrush(this->_colors->theme());
        break;
    case 3:
        painter->setBrush(this->_colors->hover(this->_colors->theme()));
        break;
    case 4:
        painter->setBrush(this->_colors->buttonBackground());
        break;
    case 5:
        painter->setBrush(this->_colors->hover(this->_colors->buttonBackground()));
        break;
    case 6:[[fallthrough]];
    case 7:
        painter->setBrush(this->_colors->disEnabled());
        break;
    }
    QPen pen(this->_colors->buttonBorder());
    painter->setPen(pen);
    painter->drawPath(clipPath);
    pen.setColor(this->_colors->buttonBottomLine());
    painter->drawLine(this->bottomLine(rect,_buttonRadius));
}

ThemePushButtonSyle::ThemePushButtonSyle(QAbstractButton *parent):
    PushButtonStyleBase(parent, false)
{
}

void ThemePushButtonSyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    if(enabled)
    {
        QColor textColor = this->_colors->autoTextColor(_colors->theme());
        painter->setPen(textColor);
        const_cast<QPalette &>(pal).setColor(textRole, textColor);
    }
    else
    {
        QColor textColor = this->_colors->disEnabled().lighter(130);
        painter->setPen(textColor);
        const_cast<QPalette &>(pal).setColor(textRole, textColor);
    }
    QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
}

void ThemePushButtonSyle::unCheckablePaint(QPainter *painter, qint8 state, const QRect &rect, const QPainterPath &clipPath) const
{
    switch (state)
    {
    case 0:[[fallthrough]];
    case 1:
        painter->setBrush(this->_colors->theme());
        break;
    case 2:
        painter->setBrush(this->_colors->hover(this->_colors->theme()));
        break;
    case 3:
        painter->setBrush(this->_colors->disEnabled());
        break;
    }

    QPen pen(this->_colors->buttonBorder());
    painter->setPen(pen);
    painter->drawPath(clipPath);
    pen.setColor(this->_colors->buttonBottomLine());
    painter->drawLine(this->bottomLine(rect,_buttonRadius));
}

RipplePushButtonStyle::RipplePushButtonStyle(QAbstractButton *parent):
    ThemePushButtonSyle(parent)
{
    parent->installEventFilter(this);
    this->_ripple = new ClickRippleAnimation(
        RIPPLE_BUTTON_TIME,
        qSqrt(qPow(parent->width()/2,2)+qPow(parent->height()/2,2)), //maxRadius
        this->_colors->theme().lighter(RIPPLE_BUTTON_LIGHTER_RATIO),
        this->_colors->theme(),
        5,
        parent
        );

    this->_ripple->setUpdate(parent);

    connect(this->_colors,&ControlColors::prominenceColorChange,this->_ripple,[this](){
        this->_ripple->updateStartColor(this->_colors->theme().lighter(RIPPLE_BUTTON_LIGHTER_RATIO));
        this->_ripple->updateEndColor(this->_colors->theme());
    });

    connect(Theme::themeObject(),&Theme::themeChange,this->_ripple,[this](){
        this->_ripple->updateStartColor(this->_colors->theme().lighter(RIPPLE_BUTTON_LIGHTER_RATIO));
        this->_ripple->updateEndColor(this->_colors->theme());
    });
}

bool RipplePushButtonStyle::eventFilter(QObject *obj, QEvent *event)
{
    QAbstractButton* parent = dynamic_cast<QAbstractButton*> (this->parent());
    if(parent)
    {
        if (obj != parent)
            return QProxyStyle::eventFilter(obj, event);

        if (event->type() == QEvent::Resize)
            _ripple->updateMaxRadius(qSqrt(qPow(parent->width() / 2, 2) + qPow(parent->height() / 2, 2)));

        if (parent->isEnabled() && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick))
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                this->_ripple->start(mouseEvent->position());
        }
    }
    return QProxyStyle::eventFilter(obj, event);
}

void RipplePushButtonStyle::unCheckablePaint(QPainter *painter, qint8 state, const QRect &rect, const QPainterPath &clipPath) const
{
    if(state == 3)
        painter->setBrush(this->_colors->disEnabled());
    else
        painter->setBrush(this->_colors->theme());
    painter->setClipPath(clipPath);
    QPen pen(this->_colors->buttonBorder());
    painter->setPen(pen);
    painter->drawPath(clipPath);
    pen.setColor(this->_colors->buttonBottomLine());
    painter->drawLine(this->bottomLine(rect,_buttonRadius));
    painter->setPen(Qt::PenStyle::NoPen);
    this->_ripple->paint(painter);
}

TransparentPushButtonStyle::TransparentPushButtonStyle(QAbstractButton *parent, bool drawBorder, bool checkable, bool preserveTransparency)
    :PushButtonStyleBase(parent, checkable),
    _drawBorder(drawBorder),
    _preserveTransparency(preserveTransparency)
{
}

void TransparentPushButtonStyle::preserveTransparency(bool preserve)
{
    this->_preserveTransparency = preserve;
}

void TransparentPushButtonStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    QAbstractButton* parent = dynamic_cast<QAbstractButton*>(this->parent());
    if(parent)
    {
        if(enabled)
        {
            if(parent->isChecked())
            {
                if(_preserveTransparency)
                {
                    const QColor& textColor = this->_colors->text();
                    painter->setPen(textColor);
                    const_cast<QPalette &>(pal).setColor(textRole, textColor);
                }
                else
                {
                    QColor textColor =this->_colors->autoTextColor(_colors->theme());
                    painter->setPen(textColor);
                    const_cast<QPalette &>(pal).setColor(textRole, textColor);
                }
            }
            else
            {
                QColor textColor =this->_colors->text();
                painter->setPen(textColor);
                const_cast<QPalette &>(pal).setColor(textRole, textColor);
            }
        }
        else
        {
            QColor textColor = this->_colors->disEnabled().lighter(130);
            painter->setPen(textColor);
            const_cast<QPalette &>(pal).setColor(textRole, textColor);
        }
    }
    QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
}

void TransparentPushButtonStyle::unCheckablePaint(QPainter *painter, qint8 state, const QRect &rect, const QPainterPath &clipPath) const
{
    switch(state)
    {
    case 0:
        painter->setBrush(_colors->transparent());
        break;
    case 1:
        painter->setBrush(_colors->transparentClick());
        break;
    case 2:
        painter->setBrush(_colors->transparentHover());
        break;
    case 3:
        painter->setBrush(this->_colors->disEnabled());
        break;
    }

    if(_drawBorder)
    {
        QPen pen(this->_colors->buttonBorder());
        painter->setPen(pen);
        painter->drawPath(clipPath);
        pen.setColor(this->_colors->buttonBottomLine());
        painter->drawLine(this->bottomLine(rect,_buttonRadius));
    }
    else
    {
        painter->setPen(Qt::NoPen);
        painter->drawPath(clipPath);
    }
}

void TransparentPushButtonStyle::checkablePaint(QPainter *painter, qint8 state, const QRect &rect, const QPainterPath &clipPath) const
{    
    switch (state)
    {
    case 0:
        painter->setBrush(_colors->transparent());
        break;
    case 1:[[fallthrough]];
    case 2:
        if (_preserveTransparency)
            painter->setBrush(_colors->transparentClick());
        else
            painter->setBrush(_colors->theme());
        break;
    case 3:
        if (_preserveTransparency)
            painter->setBrush(_colors->transparentHover());
        else
            painter->setBrush(_colors->hover(_colors->theme()));
        break;
    case 4:
        painter->setBrush(_colors->transparentClick());
        break;
    case 5:
        painter->setBrush(_colors->transparentHover());
        break;
    default:
        painter->setBrush(_colors->disEnabled());
        break;
    }

    if(_drawBorder)
    {
        QPen pen(this->_colors->buttonBorder());
        painter->setPen(pen);
        painter->drawPath(clipPath);
        pen.setColor(this->_colors->buttonBottomLine());
        painter->drawLine(this->bottomLine(rect,_buttonRadius));
    }
    else
    {
        painter->setPen(Qt::NoPen);
        painter->drawPath(clipPath);
    }
}


ToolButtonStyleBase::ToolButtonStyleBase(QAbstractButton *parent, bool checkable):
    PushButtonStyleBase(parent,checkable)
{

}

void ToolButtonStyleBase::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    //绘制背景
    if(element == QStyle::PE_PanelButtonBevel)
        return;
    return QProxyStyle::drawPrimitive(element,option,painter,widget);
}

void ToolButtonStyleBase::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    //绘制图标
    if(control==QStyle::CC_ToolButton)
    {
        QRect rect = QProxyStyle::subControlRect(QStyle::CC_ToolButton, qstyleoption_cast<const QStyleOptionComplex*>(option), QStyle::SC_ToolButton,widget);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        QPainterPath clip_path;
        clip_path.addRoundedRect(rect,_buttonRadius,_buttonRadius);
        if(this->_checkable)
            this->checkablePaint(painter,CheckableWidgetState(option->state).flage,rect,clip_path);
        else
            this->unCheckablePaint(painter,UnCheckableControlState(option->state).flage,rect,clip_path);
        painter->restore();
    }
    return QProxyStyle::drawComplexControl(control,option,painter,widget);
}

void ScrollAreaStyle::drawPrimitive(PrimitiveElement , const QStyleOption *, QPainter *, const QWidget *) const
{
    return;
}

void ScrollAreaStyle::drawControl(ControlElement , const QStyleOption *, QPainter *, const QWidget *) const
{
    return;
}


SliderStyle::SliderStyle(QWidget *parent):
    QProxyStyle(nullptr)
{
    this->_indicator = new Indicator(parent);
    parent->installEventFilter(this);
    this->setParent(parent);
}

SliderStyle::SliderStyle(QWidget *parent, Qt::Orientation orientation):
    SliderStyle(parent)
{
    this->setOrientation(orientation);
}
void SliderStyle::setOrientation(Qt::Orientation orientation)
{
    this->_indicator->_orientation = orientation;
}

bool SliderStyle::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == this->parent() && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease))
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton && event->type() == QEvent::MouseButtonPress)
            this->_indicator->mousePressEvent(mouseEvent);
        else if (event->type() == QEvent::MouseButtonRelease && !this->_indicator->geometry().contains(mouseEvent->pos()))
            this->_indicator->leaveEvent(event);
    }
    return QProxyStyle::eventFilter(obj, event);
}

QWidget *SliderStyle::indicator() const
{
    return this->_indicator;
}

void SliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if(control == QStyle::CC_Slider)
    {
        // 滑槽与指示器区域
        QRect grooveRect = QProxyStyle::subControlRect(QStyle::CC_Slider, qstyleoption_cast<const QStyleOptionComplex*>(option), QStyle::SC_SliderGroove, widget);
        QRect handleRect = QProxyStyle::subControlRect(QStyle::CC_Slider, qstyleoption_cast<const QStyleOptionComplex*>(option), QStyle::SC_SliderHandle, widget);

        auto colors = ControlColors::controlColors();
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);

        constexpr qreal penRatio = 0.278;
        bool isHorizontal = (this->_indicator->_orientation == Qt::Horizontal);

        qreal penWidth = (isHorizontal ? handleRect.height() : handleRect.width()) * penRatio;
        qreal correctXY = penWidth / 2; // 修正画笔圆角半径误差
        qreal center = (isHorizontal ? grooveRect.center().y() : grooveRect.center().x());

        QLineF primaryLine = (isHorizontal
                                  ? QLineF(correctXY, center, handleRect.left() + correctXY, center)
                                  : QLineF(center, handleRect.bottom() - correctXY, center, grooveRect.bottom() - correctXY));

        QLineF secondaryLine = (isHorizontal
                                    ? QLineF(handleRect.right() - correctXY, center, grooveRect.right() - correctXY, center)
                                    : QLineF(center, correctXY , center, handleRect.top() + correctXY));

        // 画线
        QPen pen(colors->theme(),penWidth,Qt::PenStyle::SolidLine,Qt::PenCapStyle::RoundCap);
        painter->setPen(pen);
        painter->drawLine(primaryLine);
        pen.setColor(colors->disEnabled().darker(120));
        painter->setPen(pen);
        painter->drawLine(secondaryLine);


        // 更新指示器位置
        int side = isHorizontal ? grooveRect.height() : grooveRect.width();

        int offset = (isHorizontal
                          ? handleRect.x() - (side - handleRect.width()) / 2
                          : handleRect.y() - (side - handleRect.height()) / 2);
        if(offset < 0)
            offset = 0;
        else if(isHorizontal && offset+side > widget->width())
            offset = widget->width() - grooveRect.height();

        else if(!isHorizontal && offset+side > widget->height())
                offset = widget->height() - grooveRect.width();
        QRect indRect = (isHorizontal
                             ? QRect(QPoint(offset, 0), QSize(grooveRect.height(), grooveRect.height()))
                             : QRect(QPoint(0, offset), QSize(grooveRect.width(), grooveRect.width())));

        if (this->_indicator->geometry() != indRect)
            this->_indicator->setGeometry(indRect);

        painter->restore();
        return;
    }

    return QProxyStyle::drawComplexControl(control,option,painter,widget);
}

SliderStyle::Indicator::Indicator(QWidget *parent):
    QWidget(parent)
{
    this->_ani = new SimpleAnimation(0.0,0.0,100,true,this,this);
}

void SliderStyle::Indicator::enterEvent(QEnterEvent *event)
{
    _isContains = true;
    this->_ani->setStartValue(_r/2);
    this->_ani->setEndValue(_r/1.3);
    this->_ani->start();
    return QWidget::enterEvent(event);
}

void SliderStyle::Indicator::leaveEvent(QEvent *event)
{
    _isContains = false;
    this->_ani->setStartValue(this->_ani->_runTimeValue);
    this->_ani->setEndValue(_r/2);
    this->_ani->start();
    return QWidget::leaveEvent(event);
}

void SliderStyle::Indicator::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->_ani->setStartValue(this->_ani->_runTimeValue);
        this->_ani->setEndValue(_r / 2.8);
        this->_ani->start();
    }
    QWidget::mousePressEvent(event);
}

void SliderStyle::Indicator::mouseReleaseEvent(QMouseEvent *event)
{
    if(_isContains && event->button() == Qt::LeftButton)
    {
        this->_ani->setStartValue(_r/2.5);
        this->_ani->setEndValue(_r/1.3);
        this->_ani->start();
    }
    // 不在里面直接触发leaveEvent
    return QWidget::mouseReleaseEvent(event);
}

void SliderStyle::Indicator::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QRectF rect = this->rect();
    rect.adjust(0.5,0.5,-0.5,-0.5);
    this->_r = qMin(rect.width(), rect.height())/2;
    this->_center = rect.center();
    this->_ani->_runTimeValue.setValue<qreal>(_r / 2);
}

void SliderStyle::Indicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    auto colors = ControlColors::controlColors();
    painter.setPen(QPen(colors->normalBorder(),1));
    painter.setBrush(colors->buttonBackground());
    painter.drawEllipse(_center,_r,_r);
    painter.setPen(Qt::NoPen);
    painter.setBrush(colors->theme());
    qreal r  = this->_ani->_runTimeValue.toReal();
    painter.drawEllipse(_center,r,r);
}
