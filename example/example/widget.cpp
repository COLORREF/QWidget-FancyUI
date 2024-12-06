#include "widget.h"
#include "ui_widget.h"

#include <QColorDialog>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QDesktopServices>
#include <QUrl>

#include "FancyImage.h"
#include "FancySvg.h"
#include "FancyIcon.h"
#include "FancyTitlebar.h"
#include "FancyStyle.h"
#include "FancySidebar.h"
#include "FancyLayout.h"

SidebarWidget::SidebarWidget(QWidget *parent)
    : AdaptiveLayoutWindow(parent)
{
    // 设置侧边栏
    QVBoxLayout *v1 = new QVBoxLayout(this->_sidebar);
    v1->setSpacing(0);
    v1->setContentsMargins(0, 0, 0, 0);
    this->_aniSidebar = new AnimationIndicatorSidebar(this->_sidebar);
    v1->addWidget(this->_aniSidebar);

    // 窗口基本设置
    this->setWindowTitle("FancyUI-example");
    this->setWindowIcon(QPixmap(":/resources/COLORREF.png"));
    this->resize(850,680);
    this->titleBar()->setFixedHeight(35);//调整标题栏高度，默认32

    // 自定义标题栏
    auto titleBar = this->titleBar();//获取标题栏
    auto layout =  titleBar->horizontalLayout();//获取标题栏的水平布局

    TitleBarButton* themeBtn = new TitleBarButton(titleBar);//创建一个标题栏按钮用于控制深浅主题
    themeBtn->setIcon(FancyIcon::SunMoon);
    layout->insertWidget(3,themeBtn);
    connect(themeBtn, &TitleBarButton::clicked, Theme::themeObject(), &Theme::toggleTheme);

    TitleBarButton* topBtn = new TitleBarButton(titleBar);
    topBtn->setCheckable(true);
    topBtn->setIcon(FancyIcon::Pin);
    layout->insertWidget(3,topBtn);
    connect(topBtn, &TitleBarButton::clicked,this,&AdaptiveLayoutWindow::windowTop);//选中时窗口置顶

    // 选中时动态设置置顶图标颜色
    connect(topBtn, &TitleBarButton::clicked,this,[topBtn](bool checked){
        if(checked)
            topBtn->setSvgIcon(FancySvg::replaceSvgBlack(fancyIconPath(FancyIcon::Pin),ControlColors::controlColors()->theme().name()));
        else
            topBtn->setIcon(FancyIcon::Pin, false);
    });

    // 主题色更改时如果置顶按钮选中动态更改图标颜色
    connect(ControlColors::controlColors(),&ControlColors::prominenceColorChange,this,[topBtn](){
        if(topBtn->isChecked())
            topBtn->setSvgIcon(FancySvg::replaceSvgBlack(fancyIconPath(FancyIcon::Pin),ControlColors::controlColors()->theme().name()));
    });

    // 选中时如果深浅主题切换，依然设置置顶图标颜色为当前主题色
    connect(Theme::themeObject(),&Theme::themeChange,topBtn,[topBtn](){
        if(topBtn->isChecked())
            topBtn->setSvgIcon(FancySvg::replaceSvgBlack(fancyIconPath(FancyIcon::Pin),ControlColors::controlColors()->theme().name()));
    });
}

SidebarWidget::~SidebarWidget()
{
}



Widget::Widget(QWidget *parent)
    : SidebarWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this->_clientArea);

    // 主页展示
    showHomePage();

    // 设置主题色功能
    connect(ui->pushButton, &QPushButton::clicked, this, &Widget::setThemeColor);

    // 控件效果展示
    showControl();

    // 侧边栏效果和功能展示
    showSidebar();

    // 图像处理效果展示
    showImageProcessing();

    // 图标展示（此页面功能尚未完善）
    showIcons();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::showHomePage()
{
    ui->scrollArea_2->setStyle(new ScrollAreaStyle);
    ui->scrollArea_2->style()->setParent(ui->scrollArea_2);
    QPalette p(ui->scrollArea_2->palette());
    p.setColor(QPalette::Base,Qt::transparent);
    p.setColor(QPalette::Window,Qt::transparent);
    ui->scrollArea_2->setPalette(p);

    ui->scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->widget_9->setPixmap(QPixmap(":/resources/HomeImg.jpg"));
}


void Widget::showControl()
{
    connect(ui->checkBox_2,&QCheckBox::clicked,ui->radioButton_3,&RadioButton::enableJumpAnimation);
    connect(ui->checkBox_2,&QCheckBox::clicked,ui->radioButton_4,&RadioButton::enableJumpAnimation);

    // 给按钮设置图标
    ui->pushButton_2->setIcon(FancyIcon::GlobeEarth);
    ui->pushButton_3->setIcon(FancyIcon::BlueTooth);
    ui->pushButton_4->setIcon(FancyIcon::SquarePencil);
    ui->pushButton_5->setIcon(FancyIcon::Wifi);
    ui->pushButton_6->setIcon(FancyIcon::BookMark);
    ui->pushButton_7->setIcon(FancyIcon::Fx);

    // 填充动画按钮设置
    ui->pushButton_8->setTextColor(Qt::black,Qt::white);
    ui->pushButton_8->setFillBrush(QColor(255,124,118));
    ui->pushButton_8->setBackgroundBrush(QColor(255, 124, 118, 50));
    ui->pushButton_8->setRadius(8);
    ui->pushButton_8->addShadow(0, 0, 25, QColor(255, 124, 118)); // 添加阴影

    QPointF point(ui->pushButton_9->width(),ui->pushButton_9->height());
    QRadialGradient gradient(point, ui->pushButton_9->width()/1.3, point);
    gradient.setColorAt(0.5, QColor(170, 255, 127, 150));
    gradient.setColorAt(1.0, QColor(255, 150, 150, 150));
    ui->pushButton_9->setFillBrush(QBrush(gradient));// 自定义填充画刷
    ui->pushButton_9->setBackgroundBrush(QColor(255, 124, 118, 50));
    ui->pushButton_9->addShadow(0, 0, 25, QColor(255, 124, 118)); // 添加阴影

    //设置动画类型
    ui->pushButton_9->setAnimationType(FillAnimationButton::AnimationType::CrossFill);
    ui->pushButton_10->setAnimationType(FillAnimationButton::AnimationType::DiagonalRectangle);
    ui->pushButton_11->setAnimationType(FillAnimationButton::AnimationType::BottomCircle);
    ui->pushButton_12->setAnimationType(FillAnimationButton::AnimationType::LeftRectangle);
    ui->pushButton_13->setAnimationType(FillAnimationButton::AnimationType::CrossFill);

    // 结合主题色设置填充颜色和文字颜色
    auto setColor = [this](){
        QColor thmeColor = ControlColors::controlColors()->theme();
        QColor fillTextColor = ControlColors::controlColors()->autoTextColor(thmeColor);
        ui->pushButton_10->setFillBrush(thmeColor);
        ui->pushButton_11->setFillBrush(thmeColor);
        ui->pushButton_12->setFillBrush(thmeColor);
        ui->pushButton_13->setFillBrush(thmeColor);
        ui->pushButton_9->setTextColor(Qt::black,fillTextColor);
        ui->pushButton_10->setTextColor(Qt::black,fillTextColor);
        ui->pushButton_11->setTextColor(Qt::black,fillTextColor);
        ui->pushButton_12->setTextColor(Qt::black,fillTextColor);
        ui->pushButton_13->setTextColor(Qt::black,fillTextColor);
    };
    setColor();
    connect(ControlColors::controlColors(),&ControlColors::prominenceColorChange,this,setColor);
}

void Widget::showSidebar()
{
    //创建三个指示器具有动画的侧边栏选项按钮
    auto home = new  AnimationOptionButton("主页",this->_aniSidebar,FancyIcon::Home);
    auto baseCon = new AnimationOptionButton("基础控件",this->_aniSidebar,FancyIcon::CheckBox);
    auto iamge = new AnimationOptionButton("图片处理",this->_aniSidebar,FancyIcon::Image);
    auto iocn = new AnimationOptionButton("图标",this->_aniSidebar,FancyIcon::Icons);
    auto aniCon = new AnimationOptionButton("动画效果控件",this->_aniSidebar,FancyIcon::Animation);

    //添加到侧边栏
    this->_aniSidebar->addOption(home);
    this->_aniSidebar->addOption(baseCon);
    this->_aniSidebar->addOption(iamge);
    this->_aniSidebar->addOption(iocn);
    this->_aniSidebar->addOption(aniCon);

    // 添加20个动画效果的侧边栏选项按钮，用以展示
    int number0 = static_cast<int>(FancyIcon::Number0);
    for(int i = number0; i <= number0+20; i++)
    {
        auto* btn = new AnimationOptionButton(QString("page_%1").arg(i-number0),_aniSidebar);
        btn->setIcon(FancyIcon(i));
        this->_aniSidebar->addOption(btn);
    }

    auto underSidebar = this->_aniSidebar->underrArea();//侧边栏的底部固定区域
    auto layout = dynamic_cast<QVBoxLayout*>(underSidebar->layout());//默认具有垂直布局

    // 添加两个基础的侧边栏选项按钮
    OptionButtonBase* homepage[2]{new OptionButtonBase("bilibili主页",underSidebar,FancyIcon::Bilibili),
                                  new OptionButtonBase("Github项目主页",underSidebar,FancyIcon::Github)};
    for(auto b: homepage)
    {
        b->setDrawIndicator(false);//不绘制选中指示器
        b->setCheckable(false);//不可选中
        b->correctIconCoordinates();//修正图标坐标，居中
        layout->addWidget(b);
    }

    connect(homepage[0],&QPushButton::clicked,this,[](){QDesktopServices::openUrl(QUrl(R"(https://m.bilibili.com/space/1843315943)"));});
    connect(homepage[1],&QPushButton::clicked,this,[](){QDesktopServices::openUrl(QUrl(R"(https://github.com/COLORREF/QWidget-FancyUI)"));});

    // 分页窗口切换
    connect(this->_aniSidebar,&AnimationIndicatorSidebar::optionChecked,ui->stackedWidget,&QStackedWidget::setCurrentIndex);
}

void Widget::showImageProcessing()
{
    QList<FImage> fimages;
    for(int i = 0; i < 9; i++)
        fimages.append(FImage(":/resources/Lena.png"));

    // FImage是对QImage的封装，可以无缝转换QImage、QPixmap
    // FImage的成员函数会直接修改原始图片
    ui->label_2->setPixmap(fimages[0].toQPixmap());//原图
    ui->label_3->setPixmap(fimages[1].gaussianBlur(96).toQPixmap());//高斯模糊
    ui->label_4->setPixmap(fimages[2].horizontalGaussianBlur(96).toQPixmap());//水平高斯模糊
    ui->label_5->setPixmap(fimages[3].verticalGaussianBlur(96).toQPixmap());//垂直高斯模糊
    ui->label_6->setPixmap(fimages[4].uniformBlur(96).toQPixmap());//均匀模糊
    ui->label_7->setPixmap(fimages[5].horizontalUniforBlur(96).toQPixmap());//水平均匀模糊
    ui->label_8->setPixmap(fimages[6].verticalUniforBlur(96).toQPixmap());// 垂直均匀模糊
    ui->label_9->setPixmap(fimages[7].impulseNoise(0.15).toQPixmap()); // 15%椒盐噪声
    ui->label_10->setPixmap(fimages[8].greyScale().toQPixmap());//灰度图

}

void Widget::showIcons()
{
    //图标展示（此页面功能未完善）
    ui->scrollArea->setStyle(new ScrollAreaStyle);
    ui->scrollArea->style()->setParent(ui->scrollArea);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPalette p(ui->scrollArea->palette());
    p.setColor(QPalette::Base,Qt::transparent);
    p.setColor(QPalette::Window,Qt::transparent);
    ui->scrollArea->setPalette(p);

    FlowLayout* flowLayout =  new FlowLayout(ui->scrollAreaWidgetContents);
    QButtonGroup* btnGroup = new QButtonGroup(ui->scrollAreaWidgetContents);
    flowLayout->setAnimationProperty(500,QEasingCurve::OutBack);

    constexpr auto iconCount = magic_enum::enum_count<FancyIcon>();
    // const int rows = std::ceil(static_cast<double>(iconCount) / columns); //行数
    for (int i = 0; i < iconCount; i++)
    {
        ToolButtonBase* toolBtn = new ToolButtonBase(ui->scrollAreaWidgetContents);
        toolBtn->setFixedSize(96,96);
        toolBtn->setCheckable(true);
        toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolBtn->setIcon(FancyIcon(i));
        toolBtn->setText(fancyIconName(FancyIcon(i)));
        toolBtn->setIconSize({44, 44});
        btnGroup->addButton(toolBtn);
        btnGroup->setExclusive(true);
        flowLayout->addWidget(toolBtn);
        if(i==0)toolBtn->setChecked(true);
    }
}

void Widget::setThemeColor()
{
    QColorDialog *colorDialog = new QColorDialog(ControlColors::controlColors()->prominence(), this);
    colorDialog->setWindowTitle("选择主题色");
    connect(colorDialog, &QColorDialog::colorSelected, ControlColors::controlColors(), &ControlColors::setProminence);
    connect(colorDialog, &QColorDialog::finished, colorDialog, &QColorDialog::deleteLater);
    colorDialog->show();
}
