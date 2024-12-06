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
    this->style()->setParent(this);

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

AspectRatioImageWidget::AspectRatioImageWidget(QWidget *parent, bool keepCentered, int radius):
    QWidget(parent),
    _keepCentered(keepCentered),
    _radius(radius)
{
    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void AspectRatioImageWidget::setPixmap(const QPixmap &pixmap)
{
    this->_pixmap = pixmap;
}

void AspectRatioImageWidget::setRadius(int radius)
{
    this->_radius = radius;
}

void AspectRatioImageWidget::setKeepCentered(bool keepCentered)
{
    this->_keepCentered = keepCentered;
}

void AspectRatioImageWidget::paintEvent(QPaintEvent *)
{
    if(!this->_pixmap.isNull())
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        QPainterPath path;
        path.addRoundedRect(this->rect(),this->_radius,this->_radius);
        painter.setClipPath(path);

        QPixmap draw = _pixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        if(_keepCentered)
            painter.drawPixmap(0, 0 - (draw.height() - this->height()) / 2, draw);
        else
            painter.drawPixmap(0,0,draw);
    }
}
