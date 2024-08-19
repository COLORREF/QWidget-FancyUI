#include "checkboxstyle.h"

static constexpr int TIME = 250;
static constexpr int Radius = 12;

CheckBoxStyle::CheckBoxStyle(QCheckBox *target):
    _animation(new SimpleAnimation(0,Radius,TIME,false,target)),
    _controlColors(ControlColors::controlColors()),
    _target(target)
{
    _animation->setUpdate(target);
    connect(_target,&QCheckBox::toggled,_animation,&SimpleAnimation::reverseDirectionAndStart);
}

QRect CheckBoxStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
    if(element == SE_CheckBoxIndicator)
        return QProxyStyle::subElementRect(SE_CheckBoxIndicator, option, widget).translated(1,1);
    return QProxyStyle::subElementRect(element,option,widget);
}

void CheckBoxStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_IndicatorCheckBox)
    {
        QRect indicator_rect = QProxyStyle::subElementRect(SE_CheckBoxIndicator, option, widget);
        indicator_rect.translate(1,1);
        quint8 state = 0;
        CheckableControlState s(option->state);
        if(s.normal)state = 0;// 普通状态
        if(s.normal_sunke)state = 1;// 未选中但鼠标按下
        if(s.normal_over)state = 2;// 未选中但鼠标悬浮
        if(s.selected)state = 3;// 选中状态
        if(s.select_over)state = 4;// 选中但鼠标悬浮
        if(s.select_sunken)state = 5;// 选中且鼠标按下
        if(s.unenable)state = 6; // 禁用状态
        if(s.unenable_select)state = 7;// 禁用且选中
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        // 设置画笔，绘制外矩形
        QPen pen;
        if(state == 0)
            pen = QPen(_controlColors->normalBorder());
        else if(state == 1)
            pen = QPen(_controlColors->prominence(),2);
        else if(1 < state && state < 6)
            pen = QPen(_controlColors->prominence());
        else
            pen = QPen(_controlColors->disEnabled());
        painter->setPen(pen);
        painter->drawRoundedRect(indicator_rect,3,3);

        // 设置画刷，根据状态绘制内矩形
        if( state < 6)
            painter->setBrush(_controlColors->prominence());
        else if(state > 5)
            painter->setBrush(_controlColors->disEnabled());
        painter->setPen(Qt::NoPen);
        QPainterPath clip_path;// 圆角矩形裁剪路径
        QPoint center(indicator_rect.x()+indicator_rect.width()/2,indicator_rect.y()+indicator_rect.height()/2);
        clip_path.addRoundedRect(indicator_rect, 3, 3);
        painter->setClipPath(clip_path);
        int r = _animation->_runTimeValue.toInt();
        painter->drawEllipse(center,r,r);

        // 绘制勾
        if(s.on)
        {
            int x = indicator_rect.x();
            int y = indicator_rect.y();
            int l = indicator_rect.width();
            pen = QPen(Qt::white);
            if(s.over)
                pen.setWidth(2);
            if(state == 5)
                pen.setWidth(1);
            painter->setPen(pen);
            QPointF p1(0.21875*l+x,0.53125*l+y);
            QPointF p2(0.40625*l+x,0.75*l+y);
            QPointF p3(0.84375*l+x,0.21875*l+y);
            painter->drawLine(p1,p2);
            painter->drawLine(p2,p3);
        }
        painter->restore();
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter,widget);
}
void CheckBoxStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    painter->save();
    if(!enabled)
        painter->setPen(_controlColors->disEnabled());
    QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
    painter->restore();
}
