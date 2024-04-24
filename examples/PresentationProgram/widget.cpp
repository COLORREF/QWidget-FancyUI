#include "widget.h"
#include "page_ui/ui_HoverFillButton.h"
#include "page_ui/ui_DynamicUnderlineLineEdit.h"
#include "page_ui/ui_Set.h"

Widget::Widget(QWidget *parent)
    :SidebarWindow(parent)
{
    this->setWindowTitle("展示程序");
    this->setupSidebar(this->sidebar());
    this->setupStackedWidget(this->stackedWidget());

    //设置窗口最小高度为侧边栏所有控件的累计高度,确保窗口缩放时侧边栏中的控件不会重叠
    this->setMinimumHeight(this->sidebar()->ChildsCumulativeHeight());

    connect(this->btn_expand,&QPushButton::clicked,this->sidebar(),&Sidebar::autoExpand);
    connect(this->btn_page_HoverFillButton,&SidebarOptionsButton::selectedIndex,this->stackedWidget(),&QStackedWidget::setCurrentIndex);
    connect(this->btn_page_DynamicUnderlineLineEdit,&SidebarOptionsButton::selectedIndex,this->stackedWidget(),&QStackedWidget::setCurrentIndex);
    connect(this->btn_page_Set,&SidebarOptionsButton::selectedIndex,this->stackedWidget(),&QStackedWidget::setCurrentIndex);
    connect(this,&SidebarWindow::clicked, this->sidebar(), &Sidebar::shrink);
}

Widget::~Widget()
{
    delete this->ui_HoverFillButton;
    delete this->ui_DynamicUnderlineLineEdit;
    delete this->ui_Set;
}

void Widget::setupSidebar(Sidebar *parent)
{
    parent->setBorderLinePen(QColor(64,65,66));//设置右侧分割线颜色
    parent->setIncreasedWidth(320);//设置展开增加大小

    btn_page_HoverFillButton = new SidebarOptionsButton(parent,0);//索引0
    btn_page_DynamicUnderlineLineEdit = new SidebarOptionsButton(parent,1);//索引1
    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    btn_page_Set = new SidebarOptionsButton(parent,2);//索引2
    btn_expand = new QPushButton(parent);

    //默认第一个按钮被选中
    btn_page_HoverFillButton->setChecked(true);

    //设置选中提示线颜色
    btn_page_HoverFillButton->setPromptLineColor(QColor(255,50,114));
    btn_page_DynamicUnderlineLineEdit->setPromptLineColor(QColor(255,150,0));
    btn_page_Set->setPromptLineColor(QColor(113,124,134));

    //设置按钮图标
    btn_page_HoverFillButton->setIcon(QPixmap(":/button_icon/mdi-button-pointer.svg"));
    btn_page_DynamicUnderlineLineEdit->setIcon(QPixmap(":/button_icon/line-edit.svg"));
    btn_page_Set->setIcon(QPixmap(":/button_icon/gear.svg"));

    //设置右侧文本
    btn_page_HoverFillButton->setText("HoverFillButton——悬浮填充按钮");
    btn_page_DynamicUnderlineLineEdit->setText("DynamicUnderlineLineEdit——动态下划线单行文本框");
    btn_page_Set->setText("设置");

    //展开按钮用的QPushButton,单独设置一下
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
    parent->addWidget(btn_page_HoverFillButton);
    parent->addWidget(btn_page_DynamicUnderlineLineEdit);
    parent->addItem(verticalSpacer);
    parent->addWidget(btn_page_Set);
    parent->addWidget(btn_expand);
}

void Widget::setupStackedWidget(QStackedWidget *parent)
{
    this->page_HoverFillButton = new QWidget(parent);
    this->page_DynamicUnderlineLineEdit = new QWidget(parent);
    this->page_Set = new QWidget(parent);

    //设置ui布局
    this->ui_HoverFillButton = new Ui_HoverFillButton;
    this->ui_DynamicUnderlineLineEdit = new Ui_DynamicUnderlineLineEdit;
    this->ui_Set = new Ui_Set;

    this->ui_HoverFillButton->setupUi(this->page_HoverFillButton);
    this->ui_DynamicUnderlineLineEdit->setupUi(this->page_DynamicUnderlineLineEdit);
    this->ui_Set->setupUi(this->page_Set);

    //向多页窗口添加页面
    parent->addWidget(this->page_HoverFillButton);
    parent->addWidget(this->page_DynamicUnderlineLineEdit);
    parent->addWidget(this->page_Set);

    //默认显示第一页
    parent->setCurrentIndex(0);
}


