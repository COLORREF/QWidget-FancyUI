#include "widget.h"
#include "page_ui/ui_HoverFillButton.h"
#include "page_ui/ui_DynamicUnderlineLineEdit.h"
#include "page_ui/ui_Set.h"

Widget::Widget(QWidget *parent)
    :SidebarWindow(parent)
{
    setWindowTitle("展示程序");
    setupSidebar(sidebar());
    setupStackedWidget(stackedWidget());
    connectSignals();

    //设置窗口最小高度为侧边栏所有控件的累计高度,确保窗口缩放时侧边栏中的控件不会重叠
    setMinimumHeight(sidebar()->childrenCumulativeHeight());
}

Widget::~Widget()
{
    delete ui_HoverFillButton;
    delete ui_DynamicUnderlineLineEdit;
    delete ui_Set;
}

void Widget::setupSidebar(Sidebar *parent)
{
    parent->setIncreasedWidth(320);//设置展开增加大小

    btn_page_HoverFillButton = new SidebarOptionsButton(parent);
    btn_page_DynamicUnderlineLineEdit = new SidebarOptionsButton(parent);
    verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    btn_page_Set = new SidebarOptionsButton(parent);
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
    QString style(
         R"(QPushButton{
                background:transparent;
                border:0px;
                border-radius:8px;
            }
            QPushButton:hover{
                background:rgb(233,233,233);
            }
            QPushButton:pressed{
                background:rgb(236,236,236);
            })"
        );
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
    page_HoverFillButton = new QWidget(parent);
    page_DynamicUnderlineLineEdit = new QWidget(parent);
    page_Set = new QWidget(parent);

    //设置ui布局
    ui_HoverFillButton = new Ui_HoverFillButton;
    ui_DynamicUnderlineLineEdit = new Ui_DynamicUnderlineLineEdit;
    ui_Set = new Ui_Set;

    ui_HoverFillButton->setupUi(page_HoverFillButton);
    ui_DynamicUnderlineLineEdit->setupUi(page_DynamicUnderlineLineEdit);
    ui_Set->setupUi(page_Set);

    //向多页窗口添加页面
    parent->addWidget(page_HoverFillButton);
    parent->addWidget(page_DynamicUnderlineLineEdit);
    parent->addWidget(page_Set);

    //默认显示第一页
    parent->setCurrentIndex(0);
}

void Widget::connectSignals()
{
    //批量设置索引并连接页面切换信号
    int index = 0;
    for(auto& btn : findChildren<SidebarOptionsButton*>())
    {
        btn->setIndex(index++);
        connect(btn,&SidebarOptionsButton::selectedIndex,stackedWidget(),&QStackedWidget::setCurrentIndex);
    }

    connect(btn_expand, &SidebarOptionsButton::clicked, sidebar(), &Sidebar::autoExpand);//展开和收缩
    connect(this, &SidebarWindow::clicked, sidebar(), &Sidebar::shrink);//点击页面侧边栏收缩
}


