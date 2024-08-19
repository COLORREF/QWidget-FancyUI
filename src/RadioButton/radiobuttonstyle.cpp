#include "radiobuttonstyle.h"
#include "utils/animation.h"
#include "utils/customcontrol.h"

static constexpr int Time = 150;
static constexpr int FillCircleRadius = 5;
static constexpr int HalfIndicator = 8;

RadioButtonStyle::RadioButtonStyle(QRadioButton *target):
    QProxyStyle(nullptr),
    _controlColors(ControlColors::controlColors()),
    _target(target)
{
    _group = new QParallelAnimationGroup(_target);
    _group->addAnimation(new SimpleAnimation(0,FillCircleRadius,Time,true,target));
    ((SimpleAnimation*)_group->animationAt(0))->setUpdate(_target);
    connect(_target,&QRadioButton::toggled,this,[this](bool checked){_group->setDirection((QAbstractAnimation::Direction)!checked);_group->start();});
}

void RadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_IndicatorRadioButton)
    {
        CheckableControlState s(option->state);
        if(s.unenable_select)_UnEnableOn = true;
        if(s.normal)_UnEnableOn = false;
        bool unenable_last_select = !s.enable && s.off && _UnEnableOn;

        quint8 state = 0;
        if(s.normal)state = 0;// 普通状态
        if(s.normal_over)state = 5;// 未选中但鼠标悬浮
        if(s.normal_sunke)state = 4;// 未选中但鼠标按下
        if(s.selected)state = 1;// 选中状态
        if(s.select_sunken)state = 2;// 选中且鼠标按下
        if(s.select_over)state = 3;// 选中但鼠标悬浮
        if(s.unenable)state = 6; // 禁用状态
        if(s.unenable_select)state = 7;// 禁用且选中
        if(unenable_last_select)state = 8;// 禁用未选中，但上一次是禁用且选中

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        QRect indicator(this->subElementRect(SE_RadioButtonIndicator, option, _target));
        indicator.translate(1,1);
        QPointF center(indicator.x()+indicator.width()/2,indicator.y()+indicator.height()/2);

        // 设置画笔，绘制外圆
        QPen pen;
        if(state == 0)
            pen = QPen(_controlColors->normalBorder());
        else if(state == 4)
            pen = QPen(_controlColors->prominence(), 2);
        else if(state > 5)
            pen = QPen(_controlColors->disEnabled());
        else
            pen = QPen(_controlColors->prominence());

        painter->setPen(pen);
        if(_enableJumpAnimation)
        {
            int r = ((SimpleAnimation*)_group->animationAt(1))->_runTimeValue.toInt();
            painter->drawEllipse(center,r,r);
        }
        else
            painter->drawEllipse(indicator);

        // 根据状态绘制内圆
        if(state < 4)
            painter->setBrush(_controlColors->prominence());
        else
            painter->setBrush(_controlColors->disEnabled());

        painter->setPen(Qt::NoPen);
        int r = ((SimpleAnimation*)_group->animationAt(0))->_runTimeValue.toInt();
        if(state > 6)
            r = FillCircleRadius;
        if(state == 3)
            painter->drawEllipse(center, r + 1, r + 1);
        else
            painter->drawEllipse(center, r, r);

        painter->restore();
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter,widget);
}

void RadioButtonStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    painter->save();
    if(!enabled)
        painter->setPen(_controlColors->disEnabled());
    QProxyStyle::drawItemText(painter, rect, flags, pal, enabled,text,textRole);
    painter->restore();
}

void RadioButtonStyle::enableJumpAnimation(bool isEnable)
{
    if(isEnable && !_enableJumpAnimation)
    {
        auto ani = new SimpleAnimation(HalfIndicator,HalfIndicator,Time+50,true,_target);
        ani->setKeyValueAt(0.5,0);
        ani->setUpdate(_target);
        ani->_runTimeValue = HalfIndicator;
        _group->addAnimation(ani);
        _enableJumpAnimation =true;
    }
    if(!isEnable && _enableJumpAnimation)
    {
        auto ani = _group->animationAt(1);
        _group->removeAnimation(ani);
        ani->deleteLater();
        _enableJumpAnimation = false;
    }
}
