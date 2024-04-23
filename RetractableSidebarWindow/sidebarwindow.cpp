#include "sidebarwindow.h"

SidebarWindow::SidebarWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setupUi(this);
}

Sidebar *SidebarWindow::sidebar()
{
    return this->sidebar_;
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
    this->horizontalLayout = new QHBoxLayout(parent);
    this->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    // 占位弹簧大小需要和侧边栏宽度相等,默认已相等
    this->placeholderSpring_ = new QSpacerItem(50, 0, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

    this->stackedWidget_ = new QStackedWidget(parent);

    // 添加到水平布局中
    this->horizontalLayout->addItem(placeholderSpring_);
    this->horizontalLayout->addWidget(stackedWidget_);

    this->stackedWidget_->setCurrentIndex(-1); // 没有初始页
    this->sidebar_ = new Sidebar(parent);      // 无布时,局默认位置即为(0,0)
    this->sidebar_->raise();                   // 将侧边栏移到最前方(写在其余控件之后)
    this->resize(600, 400);
}

void SidebarWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->sidebar_->resize(sidebar_->width(), height());
}

void SidebarWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->pos().x() > this->sidebar_->width())
        emit this->clicked();
}
