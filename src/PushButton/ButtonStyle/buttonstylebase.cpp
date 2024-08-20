#include "buttonstylebase.h"
#include "utils/animation.h"
#include "utils/customcontrol.h"

static constexpr int LIGHTERRATIO = 150;
static constexpr int TIME = 600;

ButtonStyleBase::ButtonStyleBase(QAbstractButton *target)
    :QProxyStyle(nullptr),
    _controlColors(ControlColors::controlColors()),
    _target(target)
{
    _target->installEventFilter(this);
    _controlColors->ExpandColors().insert("_buttonText_ButtonStyleBase",Qt::white);
    int maxRadius = qSqrt(qPow(_target->width()/2,2)+qPow(_target->height()/2,2));
    _ripple = new ClickRippleAnimation(TIME,maxRadius,_controlColors->prominence().lighter(LIGHTERRATIO),_controlColors->prominence(),5,_target);
    _ripple->setUpdate(_target);
    connect(_controlColors,&ControlColors::prominenceColorChange,_ripple,[ripple = _ripple](const QColor& color){
        ripple->updateStartColor(color.lighter(LIGHTERRATIO));
        ripple->updateEndColor(color);
    });
}

void ButtonStyleBase::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    if(enabled)
        painter->setPen(_controlColors->ExpandColors()["_buttonText_ButtonStyleBase"]);
    else
        painter->setPen(_controlColors->disEnabled().darker());
    QProxyStyle::drawItemText(painter, rect, flags, pal, enabled, text, textRole);
}

void ButtonStyleBase::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element == QStyle::CE_PushButtonBevel)
    {
        QRect rect = QProxyStyle::subElementRect(QStyle::SE_PushButtonFocusRect, option, widget);
        quint8 state = 0;
        UnCheckableControlState s(option->state);
        if(s.normal)state = 0;// 普通状态
        if(s.normal_sunke)state = 1;// 鼠标按下
        if(s.normal_over)state = 2;// 鼠标悬浮
        if(s.unenable)state = 3; // 禁用状态
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        QPainterPath clip_path;// 圆角矩形裁剪路径
        clip_path.addRoundedRect(rect,5,5);
        painter->setClipPath(clip_path);
        if(state == 3)
            painter->setBrush(_controlColors->disEnabled());
        else
            painter->setBrush(_controlColors->prominence());
        painter->setPen(QPen(Qt::GlobalColor::transparent,0));
        painter->drawRect(rect);

        this->_ripple->paint(painter);

        painter->restore();
        return;
    }
    QProxyStyle::drawControl(element,option,painter,widget);
}

bool ButtonStyleBase::eventFilter(QObject *obj, QEvent *event)
{
    if (obj != _target)
        return QProxyStyle::eventFilter(obj, event);

    if (event->type() == QEvent::Resize)
        _ripple->updateMaxRadius(qSqrt(qPow(_target->width() / 2, 2) + qPow(_target->height() / 2, 2)));

    if (_target->isEnabled() && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick))
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
            this->_ripple->start(mouseEvent->position());
    }
    return QProxyStyle::eventFilter(obj, event);
}
