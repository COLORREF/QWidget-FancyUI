#include "widget.h"
#include "FramelessWindow/framelesswindowbase.h"
#include "FramelessWindow/simpleframelesswindow.h"
#include "FramelessWindow/areowindow.h"
#include "FramelessWindow/fullytransparentwindow.h"
#include "FramelessWindow/systemcolorwindow.h"
#include "FramelessWindow/framelesswindow.h"
#include "FramelessWindow/micaeffectwindow.h"
#include "FramelessWindow/micaalteffectwindow.h"
#include "FramelessWindow/acryliceffectwindow.h"
#include "FramelessWindow/toolbutton.h"
#include "FramelessWindow/standardtitlebar.h"
#include "FramelessWindow/mswindow.h"
#include "FramelessWindow/ntqqwindow.h"
#include "FramelessWindow/adaptivelayoutwindow.h"
#include "ui_form.h"
#include "ui_example.h"

Widget::Widget(QWidget *parent)
    : QWidget{parent},
    ui{new Ui::example}
{
    this->setWindowTitle("示例程序");
    ui->setupUi(this);
    connect(ui->base, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 0));
    connect(ui->simple, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 1));
    connect(ui->standard, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 2));
    connect(ui->areo, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 3));
    connect(ui->fully_transparent, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 4));
    connect(ui->system_color, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 5));
    connect(ui->mica, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 6));
    connect(ui->mica_alt, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 7));
    connect(ui->acrylic, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 8));
    connect(ui->custom_titlebar, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 9));
    connect(ui->microsoft_store, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 10));
    connect(ui->ntqq, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 11));
    connect(ui->adaptive, &QPushButton::clicked, this, std::bind(&Widget::showExample, this, 12));
}

Widget::~Widget()
{
    for (auto &u : Uis)
        delete u;
    delete ui;
}

void Widget::showExample(int index)
{
    {
        QWidget *w;
        QWidget* master_scope;
        switch (index)
        {
        case 0:
            w = new FramelessWindowBase; // 所有无边框窗口的基类（下面所有窗口的基类），只有一个关闭按钮
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            break;
        case 1:
            w = new SimpleFramelessWindow; // 简单的无边框窗口，没有图标和标题栏文字，有最大、最小化和关闭按钮
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            break;
        case 2:
            w = new FramelessWindow; // 标准的无边框窗口
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            ((FramelessWindow *)w)->setWindowTitle("标准无边框窗口");
            break;
        case 3:
            w = new AreoWindow; // 毛玻璃窗口（可选默认构造和含参构造），若使用默认构造，则窗口无颜色
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            ((FramelessWindow *)w)->setWindowTitle("毛玻璃窗口");
            break;
        case 4:
            w = new FullyTransparentWindow; // 全透明窗口
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            ((FramelessWindow *)w)->setWindowTitle("全透明窗口");
            break;
        case 5:
            w = new SystemColorWindow; // 背景颜色跟随 Windows系统主题颜色的窗口
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            ((FramelessWindow *)w)->setWindowTitle("系统主题色窗口");
            break;
        case 6:
            w = new MicaEffectWindow; // Windows 11 云母效果窗口
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            ((FramelessWindow *)w)->setWindowTitle("Windows 11 云母窗口");
            break;
        case 7:
            w = new MicaAltEffectWindow; // Windows 11 云母Alt效果窗口
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            ((FramelessWindow *)w)->setWindowTitle("Windows 11 云母Alt窗口");
            break;
        case 8:
            w = new AcrylicEffectWindow; // Windows 11 亚克力效果窗口
            master_scope = ((FramelessWindowBase *)w)->clientArea();
            ((FramelessWindow *)w)->setWindowTitle("Windows 11 亚克力窗口");
            break;
        case 9:
            w = new FramelessWindow; // 自定义标题栏示例窗口
            ((FramelessWindow *)w)->setWindowTitle("自定义标题栏示例");
            {
                QHBoxLayout *layout = ((StandardTitleBar *)(((FramelessWindow *)w)->titleBar()))->horizontalLayout();// 获取标题栏的水平布局
                ToolButton* theme_btn = new ToolButton(w, ToolButton::ToolType::SUN_MOON);//太阳月亮按钮
                layout->insertWidget(3, theme_btn);                                   // 第三个位置插入，即最小化按钮前
                connect(theme_btn,&ToolButton::clicked,Theme::themeObject(),&Theme::toggleTheme);//自动切换深浅主题
                layout->insertItem(3, new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum)); // 弹簧
                QLineEdit *edit = new QLineEdit(w);
                edit->setStyleSheet("QLineEdit{border:1px solid rgb(123,123,123)}");
                edit->setFixedHeight(25);
                edit->setPlaceholderText("搜索");
                layout->insertWidget(3, edit);
                ToolButton *search_btn = new ToolButton(w, ToolButton::ToolType::CUSTOM); // 自定义
                search_btn->setIcon(QPixmap(":/icon/search_black.svg"));
                search_btn->setThemeIcon(QPixmap(":/icon/search_black.svg"), QPixmap(":/icon/search_white.svg"));
                search_btn->setNormalBrush(QColor(123, 123, 123));
                search_btn->setHoverBrush(QColor(123, 123, 123, 200));
                search_btn->setPressHoverBrush(QColor(123, 123, 123, 150));
                search_btn->setPressLeaveBrush(QColor(123, 123, 123));
                search_btn->setFixedSize(30, 25);
                layout->insertWidget(4, search_btn); // 单行文本框后

                w->show();
                this->hide();
                return;
            }
        case 10:
            w = new MSWindow;//微软商店风格窗口
            master_scope = ((MSWindow *)w)->clientArea();//MSWindow重新实现了qWidgetUseInSetupUi，不能使用父类的此函数
            ((FramelessWindow *)w)->setWindowTitle("微软商店风格窗口");
            break;
        case 11:
            w = new NTQQWindow;//NT_QQ(新版QQ)风格窗口
            master_scope = ((NTQQWindow *)w)->clientArea();          
            break;
        case 12:
            w = new AdaptiveLayoutWindow;//自适应布局窗口
            master_scope = ((AdaptiveLayoutWindow*)w)->clientArea();
            ((AdaptiveLayoutWindow*)w)->setWindowTitle("自适应布局窗口");
            break;
        default:
            return;
        }

        Ui::form *ui = new Ui::form;
        Uis.push_back(ui);
        ui->setupUi(master_scope);
        ui->light->setChecked(true);
        Theme::setTheme(Theme::Type::LIGHT);
        connect(ui->light, &QRadioButton::clicked, w, &Theme::toggleLight);
        connect(ui->dark, &QRadioButton::clicked, w, &Theme::toggleDark);
        connect(ui->system, &QRadioButton::clicked, w, &Theme::followSystem);
        w->setAttribute(Qt::WA_DeleteOnClose, true); // 窗口关闭（区别于hide）后，不可以再次show，必须delete，设置此属性自动delete
        connect(w, &QWidget::destroyed, this, &QWidget::show);

        if(index == 3)
        {
            QPushButton* btn = new QPushButton("设置颜色",w);
            ui->expansion->addWidget(btn);
            connect(btn,&QPushButton::clicked,w,[w](){
                QColor color = QColorDialog::getColor(QColor(255,255,255,105),w,"选择颜色",QColorDialog::ShowAlphaChannel);
                ((AreoWindow*)w)->changeColor(ABGR(color.alpha(),color.blue(),color.green(),color.red()));
            });

        }
        if(index == 11)
        {
            QCheckBox* checkbox = new QCheckBox("取消侧边栏遮罩色\n(这会使得侧边栏透明度更高)", w);
            ui->expansion->addWidget(checkbox);
            w->connect(checkbox,&QCheckBox::clicked,(NTQQWindow*)w,&NTQQWindow::setUnmaskSidebarColor);
        }

        w->show();
        this->hide();
    }
}
