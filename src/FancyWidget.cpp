#include "FancyWidget.h"
#include "FancyStyle.h"
#include "FancyImage.h"


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
            painter.drawPixmap(0 -(draw.width() - this->width()) / 2, 0 - (draw.height() - this->height()) / 2, draw);
        else
            painter.drawPixmap(0,0,draw);
    }
}

BlurredBackgroundWidget::BlurredBackgroundWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setMoveBlurre(true);
    this->setResizeBlurre(true);
    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect(this);
    blurEffect->setBlurRadius(30);
    blurEffect->setBlurHints(QGraphicsBlurEffect::BlurHint::QualityHint);
    this->setGraphicsEffect(blurEffect);
    // this->graphicsProxyWidget()
}

void BlurredBackgroundWidget::setEnableDrage(bool enbale)
{
    this->_enableDrag = enbale;
}

bool BlurredBackgroundWidget::isEnableDrag()
{
    return this->_enableDrag;
}

void BlurredBackgroundWidget::setMoveBlurre(bool enable)
{
    this->_moveBlurre = enable;
}

bool BlurredBackgroundWidget::isEnableMoveBlurre()
{
    return this->_moveBlurre;
}

void BlurredBackgroundWidget::setBlurRadius(qreal radius)
{
    QGraphicsBlurEffect *blurEffect  = dynamic_cast<QGraphicsBlurEffect *>(this->graphicsEffect());
    if(blurEffect)
        blurEffect->setBlurRadius(radius);
}

void BlurredBackgroundWidget::setBlurHints(QGraphicsBlurEffect::BlurHints hints)
{
    QGraphicsBlurEffect *blurEffect  = dynamic_cast<QGraphicsBlurEffect *>(this->graphicsEffect());
    if(blurEffect)
        blurEffect->setBlurHints(hints);
}

void BlurredBackgroundWidget::setRenderFlags(RenderFlags renderFlags)
{
    this->_renderFlags = renderFlags;
}

void BlurredBackgroundWidget::setResizeBlurre(bool enable)
{
    this->_resizeBlurre = enable;
}

bool BlurredBackgroundWidget::isEnableResizeBlurre()
{
    return this->_resizeBlurre;
}

void BlurredBackgroundWidget::clear()
{
    this->_pixmap = QPixmap();
    update();
}

void BlurredBackgroundWidget::moveEvent(QMoveEvent *event)
{
    QWidget::moveEvent(event);
    if(this->_moveBlurre)
        this->blurre(this->parentWidget(),this->geometry());
}

void BlurredBackgroundWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (_enableDrag && event->button() == Qt::LeftButton)
    {
        _isDragging = true;
        _dragStartPos = event->pos(); // 记录鼠标相对控件的位置
    }
}

void BlurredBackgroundWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if (_enableDrag && _isDragging)
        this->move(this->pos() + (event->pos() - _dragStartPos));
}

void BlurredBackgroundWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if (_enableDrag && event->button() == Qt::LeftButton)
        _isDragging = false; // 停止拖动
}

void BlurredBackgroundWidget::paintEvent(QPaintEvent *)
{

    // QPainterPath path;
    // path.addRoundedRect(this->rect(),18,18);
    // painter.setClipPath(path);
    if(!this->_pixmap.isNull())
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.drawPixmap(0,0,_pixmap);

        painter.fillRect(rect(),ControlColors::controlColors()->transparentHover().darker(100));
    }
    // painter.setPen(ControlColors::controlColors()->buttonBorder());
    // painter.drawPath(path);
}

void BlurredBackgroundWidget::resizeEvent(QResizeEvent *event)
{
    if(this->_resizeBlurre)
        this->blurre(this->parentWidget(),this->geometry());
    QWidget::resizeEvent(event);
}

void BlurredBackgroundWidget::blurre(QWidget *device, QRect rect)
{
    QTimer::singleShot(0,this,[=](){
        const qreal dpr = devicePixelRatioF(); // 获取设备的像素比
        QPixmap res((QSizeF(rect.size()) * dpr).toSize());
        res.setDevicePixelRatio(dpr);// 设置 QPixmap 像素比
        res.fill(Qt::transparent);
        device->render(&res,QPoint(), QRegion(rect), this->_renderFlags);
        this->_pixmap = res;
        // FImage fim(res.toImage());
        // this->_pixmap = fim.gaussianBlur(60).toQPixmap();
        this->update();
    });
}

