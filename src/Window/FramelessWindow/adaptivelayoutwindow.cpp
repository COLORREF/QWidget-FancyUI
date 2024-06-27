#include "adaptivelayoutwindow.h"
#include "FramelessWindow/toolbutton.h"

AdaptiveLayoutWindow::AdaptiveLayoutWindow(QWidget *parent, WindowEffectType effectType,bool stackWindow)
    :MSWindow{parent,effectType}
{
    this->_isExpand = true;
    this->_animation = new QVariantAnimation(this);
    this->_animation->setDuration(200);
    this->_animation->setStartValue(300);
    this->_animation->setEndValue(50);

    connect(this->_animation,&QVariantAnimation::valueChanged,this,[=](const QVariant& value){
        this->_sidebar->setFixedWidth(value.value<int>());
    });

    connect(this->_animation,&QVariantAnimation::finished,this,[=](){
        this->_isExpand = !this->_isExpand;
        this->checkWidth();
    });
    this->_sidebar->setFixedWidth(300);

    if(stackWindow)
    {
        ToolButton* left_arrow = new ToolButton(this,ToolButton::ToolType::LEFT_ARROW);
        left_arrow->setFixedWidth(35);
        this->_titleBar->horizontalLayout()->insertSpacerItem(0, new QSpacerItem(30, 0, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
    }
}
void AdaptiveLayoutWindow::checkWidth()
{
    int w = this->width();
    if( w < 850 && this->_animation->state() != QVariantAnimation::State::Running && this->_isExpand)
    {
        this->_animation->setDirection(QVariantAnimation::Direction::Forward);
        this->_animation->start();
    }

    else if( w > 850 && this->_animation->state() != QVariantAnimation::State::Running && !this->_isExpand)
    {
        this->_animation->setDirection(QVariantAnimation::Direction::Backward);
        this->_animation->start();
    }
}

F_DEFINITION_RESIZEEVENT(AdaptiveLayoutWindow)
{
    this->checkWidth();
    QWidget::resizeEvent(event);
}
