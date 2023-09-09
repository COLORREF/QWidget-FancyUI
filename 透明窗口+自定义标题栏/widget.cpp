#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , exitbtn(new QPushButton(this))
    , maxsize(new QPushButton(this))
    , minisize(new QPushButton(this))
    ,min_W_size(QSize(320,330))

{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
    setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明
    setMinimumSize(min_W_size);
    setMouseTracking(true);//打开鼠标跟踪功能，以便在鼠标移动时触发mouseMoveEvent事件而不需要按键按下，因为QWidget没有鼠标悬停事件

    //水平分割线
    line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);//水平分割线
    line->setFrameShadow(QFrame::Sunken);//设置线的阴影效果
    line->move(0,30);
    line->resize(width(),1);

    //关闭按钮
    exitbtn->resize(47, 30);
    exitbtn->setText("✖️");
    exitbtn->move(width()-exitbtn->width(),0);
    exitbtn->setStyleSheet("QPushButton {"
                           "    color: white;"
                           "    border: none;"
                           "    border-top-right-radius:"+QString::number(border_radius)+"px;"
                           "    background-color: rgba(0,0,0,0); "
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(196,43,28,255);"
                           "}");
    connect(exitbtn,&QPushButton::clicked,this,[]{exit(0);});

    //最大化按钮
    maxsize->resize(47, 30);
    maxsize->setText("🗖");
    maxsize->move(width() - exitbtn->width() - maxsize->width(), 0);
    maxsize->setStyleSheet("QPushButton {"
                           "    color: white;"
                           "    border: none;"
                           "    background-color: rgba(0,0,0,0); "
                           "}"
                           "QPushButton:hover {"
                           "    background-color: rgba(229,234,239,50);"
                           "}");
    connect(maxsize,&QPushButton::clicked,this,[=](){
        if(!isMaximized())
            showMaximized();
        else
            showNormal();});

    //最小化按钮
    minisize->resize(47, 30);
    minisize->setText("🗕");
    minisize->move(width() - exitbtn->width() - maxsize->width() - minisize->width(), 0);
    minisize->setStyleSheet("QPushButton {"
                            "    color: white;"
                            "    border: none;"
                            "    background-color: rgba(0,0,0,0); "
                            "}"
                            "QPushButton:hover {"
                            "    background-color: rgba(229,234,239,50);"
                            "}");
    connect(minisize,&QPushButton::clicked,this,&QWidget::showMinimized);

    //控制全局透明度
    connect(ui->horizontalSlider,&QSlider::valueChanged,this,[=](int value) {
        setWindowOpacity((static_cast<float>(value) / 100.0)); });

    //R
    connect(ui->horizontalSlider_3,&QSlider::valueChanged,this,[=](int value) {
        bkR = value;
        update();});
    //G
    connect(ui->horizontalSlider_4,&QSlider::valueChanged,this,[=](int value) {
        bkG = value;
        update();});
    //B
    connect(ui->horizontalSlider_5,&QSlider::valueChanged,this,[=](int value) {
        bkB = value;
        update();});
    //A
    connect(ui->horizontalSlider_2,&QSlider::valueChanged,this,[=](int value) {
        bkA = value;
        update();});


    //窗口置顶
    //注意：SetWindowPos函数，在Qt Creator中使用MSVC编译无法通过，需要使用MinGW编译，或使用静态编译，静态编译MSVC可以通过
    //此方法的优点：不会闪屏，无需刷新窗口
    //Qt提供了相关函数，但是需要刷新窗口，窗口会闪一下
    //缺点：使用了Windows Api，无法跨平台
    connect(ui->checkBox_2,&QCheckBox::clicked,this,[=](bool isclicked){
        if(isclicked)
            SetWindowPos(reinterpret_cast<HWND>(winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//窗口置顶
        else
            SetWindowPos(reinterpret_cast<HWND>(winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//取消置顶
    });

    //禁止捕获
    //Qt没有提供相关函数，必须使用对应平台的api
    connect(ui->checkBox_3,&QCheckBox::clicked,this,[=](bool isclicked){
        if(isclicked)
            SetWindowDisplayAffinity(reinterpret_cast<HWND>(winId()), WDA_EXCLUDEFROMCAPTURE);//禁止捕获
        else
            SetWindowDisplayAffinity(reinterpret_cast<HWND>(winId()), WDA_NONE);//恢复
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setBrush(QBrush(QColor(bkR,bkG,bkB,bkA)));//背景色
    painter.setPen(Qt::transparent);//rgba(0,0,0,0)
    painter.setRenderHint(QPainter::Antialiasing);//抗锯齿
    QPainterPath painterPath;
    painterPath.addRoundedRect(rect(),border_radius,border_radius);//添加绘制区域为窗口区域，并设置圆角
    painter.drawPath(painterPath);//绘制
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    m_click = event->globalPosition();//记录鼠标点击坐标
    w_left_top = frameGeometry().topLeft();
    w_left_bottom = frameGeometry().bottomLeft();
    w_right_top = frameGeometry().topRight();
    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    QRect mainRect(boundary_width,boundary_width,width()-2*boundary_width,height()-2*boundary_width);
    if(!mainRect.contains(event->pos()) && !isdirectionLock && !ismove)//光标在边框区域,并且改变的方向非锁定状态,并且不是在移动窗口
    {
        isresize = true;
        //要先判断四个角,如果只使用一组if-else
        //左上角
        if(event->pos().x() < boundary_width && event->pos().y() < boundary_width)
        {
            setCursor(Qt::SizeFDiagCursor);//设置光标样式
            resize_direction = direction::left_top;
        }
        //左下角
        else if(event->pos().x() < boundary_width && event->pos().y() > boundary_width + mainRect.height())
        {
            setCursor(Qt::SizeBDiagCursor);
            resize_direction = direction::left_bottom;
        }
        //右上角
        else if(event->pos().x() > boundary_width + mainRect.width() && event->pos().y() < boundary_width)
        {
            setCursor(Qt::SizeBDiagCursor);
            resize_direction = direction::right_top;
        }
        //右下角
        else if(event->pos().x() > boundary_width + mainRect.width() && event->pos().y() > boundary_width + mainRect.height())
        {
            setCursor(Qt::SizeFDiagCursor);
            resize_direction = direction::right_bottom;
        }
        //左边框
        else if(event->pos().x() < boundary_width)
        {
            setCursor(Qt::SizeHorCursor);
            resize_direction = direction::left;
        }
        //右边框
        else if(event->pos().x() > boundary_width + mainRect.width())
        {
            setCursor(Qt::SizeHorCursor);
            resize_direction = direction::right;
        }
        //上边框
        else if(event->pos().y() < boundary_width)
        {
            setCursor(Qt::SizeVerCursor);
            resize_direction = direction::top;
        }
        //下边框
        else if(event->pos().y() > boundary_width + mainRect.height())
        {
            setCursor(Qt::SizeVerCursor);
            resize_direction = direction::bottom;
        }
    }

    if(mainRect.contains(event->pos()) && event->buttons() == Qt::NoButton) //鼠标在主区域内,并且没有任何按钮按下
    {
        setCursor(Qt::ArrowCursor);//恢复鼠标样式
        isresize = false;
        isdirectionLock = false;
        ismove = false;
    }

    if(!isresize)//不是在调整大小，即移动窗口或无操作
    {
        isdirectionLock = false;//改变方向变成非锁定状态
        resize_direction = direction::none;
        if (event->buttons() == Qt::LeftButton && event->pos().y() <= 30 && !ui->checkBox->isChecked())//仅标题栏可移动
        {
            ismove = true;
            setCursor(Qt::SizeAllCursor);
            QPointF newPos = event->globalPosition()-m_click+w_left_top;
            move(newPos.toPoint()); // 移动窗口到新位置
        }
        else if(event->buttons() == Qt::LeftButton  && ui->checkBox->isChecked())//全屏可移动
        {
            ismove = true;
            setCursor(Qt::SizeAllCursor);
            QPointF newPos = event->globalPosition()-m_click+w_left_top;
            move(newPos.toPoint()); // 移动窗口到新位置
        }
    }

    if(isresize && event->buttons() & Qt::LeftButton)//在调整大小
    {
        isdirectionLock = true;//鼠标点击后变成锁定状态
        ismove = false;
        switch(resize_direction)
        {
        case direction::left://左边框
        {
            int dx =m_click.x()-event->globalPosition().x();//改变量
            int new_x = w_left_top.x()-dx;//新的x坐标
            setGeometry(new_x,w_left_top.y(),dx+w_right_top.x()-w_left_top.x(),height());//设置新的位置和大小
            update();
            break;
        }
        case direction::right://右边框
        {
            int dx = event->globalPosition().x()-m_click.x();
            resize(dx+w_right_top.x()-w_left_top.x(),height());//向右改变大小不需要移动窗口
            update();
            break;
        }
        case direction::top://上边框
        {
            int dy = m_click.y() - event->globalPosition().y();
            int new_y = w_left_top.y()-dy;
            setGeometry(w_left_top.x(),new_y,width(),dy+w_left_bottom.y()-w_left_top.y());
            update();
            break;
        }
        case direction::bottom://下边框
        {
            int dy = event->globalPosition().y()-m_click.y();
            resize(width(),dy+w_left_bottom.y()-w_left_top.y());
            update();
            break;
        }
        case direction::left_top://左上角
        {
            int dx =m_click.x()-event->globalPosition().x();//改变量
            int new_x = w_left_top.x()-dx;//新的x坐标
            int dy = m_click.y() - event->globalPosition().y();
            int new_y = w_left_top.y()-dy;
            setGeometry(new_x,new_y,dx+w_right_top.x()-w_left_top.x(),dy+w_left_bottom.y()-w_left_top.y());
            update();
            break;
        }
        case direction::left_bottom://左下角
        {
            int dx =m_click.x()-event->globalPosition().x();//改变量
            int new_x = w_left_top.x()-dx;//新的x坐标
            int dy = event->globalPosition().y()-m_click.y();
            setGeometry(new_x,w_left_top.y(),dx+w_right_top.x()-w_left_top.x(),dy+w_left_bottom.y()-w_left_top.y());
            update();
            break;
        }
        case direction::right_top://右上角
        {
            int dx = event->globalPosition().x()-m_click.x();
            int dy = m_click.y() - event->globalPosition().y();
            int new_y = w_left_top.y()-dy;
            setGeometry(w_left_top.x(),new_y,dx+w_right_top.x()-w_left_top.x(),dy+w_left_bottom.y()-w_left_top.y());
            update();
            break;
        }
        case direction::right_bottom://右下角
        {
            int dx = event->globalPosition().x()-m_click.x();
            int dy = event->globalPosition().y()-m_click.y();
            resize(dx+w_right_top.x()-w_left_top.x(),dy+w_left_bottom.y()-w_left_top.y());
            update();
            break;
        }
        default://none
            break;
        }
    }

    QWidget::mouseMoveEvent(event);
}

void Widget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    line->resize(width(),1);
    exitbtn->move(width()-exitbtn->width(),0);
    maxsize->move(width() - exitbtn->width() - maxsize->width(), 0);
    minisize->move(width() - exitbtn->width() - maxsize->width() - minisize->width(), 0);

    if(isMaximized())
    {
        border_radius = 0;
        exitbtn->setStyleSheet("QPushButton {"
                               "    color: white;"
                               "    border: none;"
                               "    border-top-right-radius: 0px;"
                               "    background-color: rgba(0,0,0,0); "
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(196,43,28,255);"
                               "}");
    }
    else
    {
        border_radius = 15;
        exitbtn->setStyleSheet("QPushButton {"
                               "    color: white;"
                               "    border: none;"
                               "    border-top-right-radius:"+QString::number(border_radius)+"px;"
                               "    background-color: rgba(0,0,0,0); "
                               "}"
                               "QPushButton:hover {"
                               "    background-color: rgba(196,43,28,255);"
                               "}");
    }
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    if(event->button() == Qt::LeftButton)
    {
        if(!isMaximized() && ui->checkBox->isChecked())
            showMaximized();
        else if(!isMaximized() && event->pos().y() <= 30)
            showMaximized();
        else
            showNormal();
    }

}



