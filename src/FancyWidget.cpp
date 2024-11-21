#include "FancyWidget.h"
#include "FancyStyle.h"


InertiaScrollArea::InertiaScrollArea(QWidget *parent):
    QScrollArea(parent),
    _scrollContentWidget{new QWidget(this)},
    _scrollContentWidgetVerticalLayout(new QVBoxLayout(this->_scrollContentWidget))
{
    this->setWidgetResizable(true);
    this->setWidget(this->_scrollContentWidget);
    this->_scrollContentWidgetVerticalLayout->setSpacing(2);
    this->_scrollContentWidgetVerticalLayout->setContentsMargins(0, 0, 0, 0);
    this->setStyle(new ScrollAreaStyle);

    // 惯性平滑滚动
    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(this);
    QScrollerProperties scrollerProperties;
    scrollerProperties.setScrollMetric(QScrollerProperties::ScrollMetric::ScrollingCurve, QEasingCurve::OutQuad);
    scrollerProperties.setScrollMetric(QScrollerProperties::ScrollMetric::DecelerationFactor, 0.9); // 减小减速，增加惯性效果
    scroller->setScrollerProperties(scrollerProperties);



    QPalette p(this->palette());
    p.setColor(QPalette::Base,Qt::transparent);
    p.setColor(QPalette::Window,Qt::transparent);
    this->setPalette(p);
}
