#include "widget.h"
#include "ui_page.h"

Widget::Widget(QWidget *parent)
    :SidebarWindow(parent)
{
    this->setWindowTitle("伸缩侧边栏窗口");
    this->setupSidebar(this->sidebar());
    this->setupStackedWidget(this->stackedWidget());

    //设置窗口最小高度为侧边栏所有控件的累计高度,确保窗口缩放时侧边栏中的控件不会重叠
    this->setMinimumHeight(this->sidebar()->ChildsCumulativeHeight());

    connect(this->btn_expand,&QPushButton::clicked,this->sidebar(),&Sidebar::autoExpand);
    connect(this->btn_page_1,&SidebarOptionsButton::selectedIndex,this->stackedWidget(),&QStackedWidget::setCurrentIndex);
    connect(this->btn_page_2,&SidebarOptionsButton::selectedIndex,this->stackedWidget(),&QStackedWidget::setCurrentIndex);
    connect(this->btn_page_3,&SidebarOptionsButton::selectedIndex,this->stackedWidget(),&QStackedWidget::setCurrentIndex);
    connect(this->btn_set,&SidebarOptionsButton::selectedIndex,this->stackedWidget(),&QStackedWidget::setCurrentIndex);
    connect(this,&SidebarWindow::clicked, this->sidebar(), &Sidebar::shrink);
}

Widget::~Widget()
{
    delete this->page_ui;
}

void Widget::setupSidebar(Sidebar *parent)
{
    btn_page_1 = new SidebarOptionsButton(parent,0);
    btn_page_2 = new SidebarOptionsButton(parent,1);
    btn_page_3 = new SidebarOptionsButton(parent,2);
    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    btn_set = new SidebarOptionsButton(parent,3);
    btn_expand = new QPushButton(parent);

    //默认第一个按钮被选中
    btn_page_1->setChecked(true);

    //设置选中提示线颜色
    btn_page_1->setPromptLineColor(QColor(255,50,114));
    btn_page_2->setPromptLineColor(QColor(0,174,236));
    btn_page_3->setPromptLineColor(QColor(255,198,46));
    btn_set->setPromptLineColor(QColor(113,124,134));

    //设置按钮图标
    btn_page_1->setIcon(QPixmap(":/button_icon/Like.svg"));
    btn_page_2->setIcon(QPixmap(":/button_icon/coins.svg"));
    btn_page_3->setIcon(QPixmap(":/button_icon/collect.svg"));
    btn_set->setIcon(QPixmap(":/button_icon/gear.svg"));

    //设置右侧文本
    btn_page_1->setText("😚给up点个赞可以吗~~");
    btn_page_2->setText("🪙投个币吧~~");
    btn_page_3->setText("😍要不要收藏一下❓");
    btn_set->setText("设置");

    //展开按钮用的QPushButton,单独设置一下(也可以用sidebarOptionsButton)
    QString style(R"(
            QPushButton{
                background:transparent;
                border:0px;
                border-radius:8px;
            }
            QPushButton:hover{
                background:rgb(233,233,233);
            }
            QPushButton:pressed{
                background:rgb(236,236,236);
            }
        )");
    btn_expand->setFixedSize(40,36);
    btn_expand->setStyleSheet(style);
    btn_expand->setIcon(QIcon(QPixmap(":/button_icon/three_lines.svg")));

    // 添加到sidebar中（Sidebar默认的垂直布局,注意添加顺序:从上到下）
    parent->addWidget(btn_page_1);
    parent->addWidget(btn_page_2);
    parent->addWidget(btn_page_3);
    parent->addItem(verticalSpacer);
    parent->addWidget(btn_set);
    parent->addWidget(btn_expand);
}

void Widget::setupStackedWidget(QStackedWidget *parent)
{
    this->page_ui = new Ui_page;

    page_1 = new QWidget(parent);
    page_2 = new QWidget(parent);
    page_3 = new QWidget(parent);
    page_set = new QWidget(parent);

    //向多页窗口添加页面
    parent->addWidget(page_1);
    parent->addWidget(page_2);
    parent->addWidget(page_3);
    parent->addWidget(page_set);

    //默认显示第一页
    parent->setCurrentIndex(0);

    //简单设置一下背景色
    parent->setStyleSheet("background-color: rgb(249,249,249);");

    //设置ui布局
    page_ui->setupUi(page_1);
    page_ui->setupUi(page_2);
    page_ui->setupUi(page_3);
    page_ui->setupUi(page_set);

    //设置展示页面中label的内容(展示页面只有一个label,特殊处理🤔)
    page_1->findChildren<QLabel*>().at(0)->setText("给up点个😚赞可以吗~~");
    page_2->findChildren<QLabel*>().at(0)->setText("😚投个币吧~~🪙");
    page_3->findChildren<QLabel*>().at(0)->setText("要不要😍收藏一下❓");
    page_set->findChildren<QLabel*>().at(0)->setText("设置~~");
}


