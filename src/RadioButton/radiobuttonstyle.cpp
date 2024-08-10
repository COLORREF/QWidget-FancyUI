#include "radiobuttonstyle.h"

RadioButtonStyle::RadioButtonStyle(QRadioButton *target):
    QProxyStyle(nullptr),
    _animation(new SimpleAnimation(0,5,150,false,target)),// 最大半径为5,150毫秒完成动画
    _controlColors(ControlColors::controlColors()),
    _radius(_animation->startValue().toInt()),
    _target(target)
{
    _animation->setUpdate(target);
    connect(_animation,&QVariantAnimation::valueChanged,this,[r = &_radius](const QVariant &value)mutable{*r = value.toInt();});
    connect(_target,&QRadioButton::toggled,_animation,&SimpleAnimation::reverseDirectionAndStart);
}

QRect RadioButtonStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    if(element == SE_RadioButtonIndicator)
        return QProxyStyle::subElementRect(SE_RadioButtonIndicator, option, widget).translated(1,1);
    return QProxyStyle::subElementRect(element,option,widget);
}

void RadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_IndicatorRadioButton)
    {
        QStyle::State op_state = option->state;
        QRect indicator = subElementRect(SE_RadioButtonIndicator, option, widget);
        QPoint center(indicator.x()+indicator.width()/2,indicator.y()+indicator.height()/2);
        quint8 state = 0;

        bool enable = op_state & State_Enabled;
        bool on = op_state & State_On;
        bool off = op_state & State_Off;
        bool over = op_state & State_MouseOver;
        bool sunken = op_state & State_Sunken;

        bool normal = enable && off && !over && !sunken;
        bool normal_over = enable && off && over && !sunken;
        bool normal_sunke = enable && off && over && sunken;

        bool selected = enable && on && !over && !sunken;
        bool select_sunken = enable && on && over && sunken;
        bool select_over = enable && on && over && !sunken;

        bool unenable = !enable && off;
        bool unenable_select = !enable && on;
        if(unenable_select)_UnEnableOn = true;
        if(normal)_UnEnableOn = false;
        bool unenable_last_select = !enable && off && _UnEnableOn;

        if(normal)state = 0;// 普通状态
        if(normal_over)state = 5;// 未选中但鼠标悬浮
        if(normal_sunke)state = 4;// 未选中但鼠标按下
        if(selected)state = 1;// 选中状态
        if(select_sunken)state = 2;// 选中且鼠标按下
        if(select_over)state = 3;// 选中但鼠标悬浮
        if(unenable)state = 6; // 禁用状态
        if(unenable_select)state = 7;// 禁用且选中
        if(unenable_last_select)state = 8;// 禁用未选中，但上一次是禁用且选中

        painter->save(); // ① save
        painter->setRenderHint(QPainter::Antialiasing, true);

        // 设置画笔，绘制外圆
        QPen pen;
        int adjustedRadius = _radius;

        if(state == 0)
            pen = QPen(_controlColors->normalBorder());
        else if(state == 4)
            pen = QPen(_controlColors->prominence(), 2);
        else if(state > 5)
            pen = QPen(_controlColors->disEnabled());
        else
            pen = QPen(_controlColors->prominence());
        if(state > 6)
            adjustedRadius = _animation->endValue().toInt();
        painter->setPen(pen);
        painter->drawEllipse(indicator);

        // 根据状态绘制内圆
        if(state < 4)
            painter->setBrush(_controlColors->prominence());
        else
            painter->setBrush(_controlColors->disEnabled());
        painter->setPen(Qt::NoPen);
        if(state == 3)
            painter->drawEllipse(center, adjustedRadius + 1, adjustedRadius + 1);
        else
            painter->drawEllipse(center, adjustedRadius, adjustedRadius);
        painter->restore();
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter,widget);
}

void RadioButtonStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    if(!enabled)
        const_cast<QPalette &>(pal).setColor(textRole,_controlColors->disEnabled());
    return QProxyStyle::drawItemText(painter, rect, flags, pal, enabled,text,textRole);
}
