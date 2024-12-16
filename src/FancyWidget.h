#ifndef FANCYWIDGET_H
#define FANCYWIDGET_H


#include <QWidget>
#include <QSpacerItem>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QScroller>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QMoveEvent>
#include <QGraphicsBlurEffect>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWindow>
#include <QTimer>
#include <QMouseEvent>
#include "precompile.h"
#include "FancyStyle.h"

// 垂直布局惯性滚动区域，默认带有控件区域
class InertiaScrollArea : public QScrollArea
{
public:
    explicit InertiaScrollArea(QWidget *parent);
public:
    inline void addItem(QLayoutItem *item){this->_scrollContentWidgetVerticalLayout->addItem(item);}
    inline void insertItem(int index, QLayoutItem *item){this->_scrollContentWidgetVerticalLayout->insertItem(index, item);}
    inline void addWidget(QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment())
    {this->_scrollContentWidgetVerticalLayout->addWidget(widget, stretch, alignment);}
    inline void insertWidget(int index, QWidget *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment())
    {this->_scrollContentWidgetVerticalLayout->insertWidget(index, widget, stretch, alignment);}

    F_PROTECTED_PROPERTY(QWidget* ,scrollContentWidget)
    F_PROTECTED_PROPERTY(QVBoxLayout*, scrollContentWidgetVerticalLayout)
};

class AspectRatioImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AspectRatioImageWidget(QWidget* parent, bool keepCentered = true, int radius = 10);
    void setPixmap(const QPixmap& pixmap);
    void setRadius(int radius);
    void setKeepCentered(bool keepCentered);
protected:
    void paintEvent(QPaintEvent*) override;
private:
    QPixmap _pixmap;
    bool _keepCentered;
    int _radius;
};

class BlurredBackgroundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BlurredBackgroundWidget(QWidget* parent);

    void setEnableDrage(bool enbale);
    bool isEnableDrag();
    void setMoveBlurre(bool enable);
    bool isEnableMoveBlurre();
    void setBlurRadius(qreal blurRadius);
    void setBlurHints(QGraphicsBlurEffect::BlurHints hints);
    void setRenderFlags(QWidget::RenderFlags renderFlags);

    void setResizeBlurre(bool enable);
    bool isEnableResizeBlurre();

    void clear();
protected:
    void moveEvent(QMoveEvent*event)override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent*event)override;
    void resizeEvent(QResizeEvent*event)override;

private:
    QPixmap _pixmap;
    bool _moveBlurre = false;
    bool _resizeBlurre = false;
    bool _enableDrag = false;
    QWidget::RenderFlags _renderFlags = /*QWidget::DrawWindowBackground |*/ QWidget::DrawChildren |  QWidget::IgnoreMask;
    bool _isDragging;
    QPoint _dragStartPos; // 鼠标按下时的位置
public slots:
    void blurre(QWidget* device, QRect rect);

};


// 测试功能，未完善
// 模糊顶部导航栏滚动区
class BlurredNavigationBarScrollArea : public QScrollArea
{
public:
    explicit BlurredNavigationBarScrollArea(QWidget *parent):
        QScrollArea(parent),
        _externalContentWidget{new QWidget(this)},
        _externalLayout(new QVBoxLayout(this->_externalContentWidget)),
        _verticalSpacer(new QSpacerItem(20, 500, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed)),
        _scrollContentWidget(new QWidget(this->_externalContentWidget)),
        _navigationBar(new BlurredBackgroundWidget(this))
    {
        this->setWidgetResizable(true);
        this->setWidget(this->_externalContentWidget);
        this->_externalLayout->setSpacing(0);
        this->_externalLayout->setContentsMargins(0, 0, 0, 0);
        this->_externalLayout->addItem(this->_verticalSpacer);
        this->_externalLayout->addWidget(this->_scrollContentWidget);

        this->setStyle(new ScrollAreaStyle);
        this->style()->setParent(this);
        QPalette p(this->palette());
        p.setColor(QPalette::Base,Qt::transparent);
        p.setColor(QPalette::Window,Qt::transparent);
        this->setPalette(p);

        connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this,[this](){
            this->_navigationBar->clear();
            this->_navigationBar->blurre(this,this->_navigationBar->geometry());
        });
        connect(Theme::themeObject(), &Theme::themeChange, this,[this](){
            this->_navigationBar->clear();
            this->_navigationBar->blurre(this,this->_navigationBar->geometry());
        });
    }

    QWidget* widget()
    {
        return this->_scrollContentWidget;
    }

    void resizeEvent(QResizeEvent* event)override
    {
        QScrollArea::resizeEvent(event);
        _navigationBar->resize(this->_externalContentWidget->width(),this->_verticalSpacer->sizeHint().height());
        // _navigationBar->move(0,0);
    }

private:
    QWidget* _externalContentWidget;
    QVBoxLayout* _externalLayout;
    QSpacerItem * _verticalSpacer;
    QWidget* _scrollContentWidget;
    BlurredBackgroundWidget* _navigationBar;
};

#endif // FANCYWIDGET_H
