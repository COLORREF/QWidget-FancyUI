#include "sidebarwindow.h"

SidebarWindow::SidebarWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

Sidebar *SidebarWindow::sidebar()
{
    return sidebar_;
}

QSpacerItem *SidebarWindow::placeholderSpring()
{
    return placeholderSpring_;
}

QStackedWidget *SidebarWindow::stackedWidget()
{
    return stackedWidget_;
}

void SidebarWindow::setupUi(QWidget *parent)
{
    horizontalLayout = new QHBoxLayout(parent);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    //占位弹簧大小需要和侧边栏宽度相等,默认已相等
    placeholderSpring_ = new QSpacerItem(50, 0, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

    stackedWidget_ = new QStackedWidget(parent);

    //添加到水平布局中
    horizontalLayout->addItem(placeholderSpring_);
    horizontalLayout->addWidget(stackedWidget_);

    //没有初始页
    stackedWidget_->setCurrentIndex(-1);

    //无布时,局默认位置即为(0,0)
    sidebar_ = new Sidebar(parent);

    //将侧边栏移到最前方(写在其余控件之后)
    sidebar_->raise();

    resize(600,400);
}

void SidebarWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    sidebar_->resize(sidebar_->width(),height());
}

void SidebarWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if( event->pos().x() > this->sidebar_->width())
        emit this->clicked();
}
