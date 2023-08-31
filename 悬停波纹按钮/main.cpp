/*************************
 * Qt版本：Qt 6.4.2
 * 使用qmake构建项目
 * 完成时间：2023/8/30
 * 更新时间：2023/8/31 第一次更新：修bug
 ****************************************
 * **用C++重写Python的代码
 * **Python代码作者：B站：Rev_RoastDuck UID：439475163
 * **C++代码作者：B站：EMC方 UID：1843315943
 * **C++版本 GitHub：https://github.com/BFEMCC/Qt-widget-Fancy_UI
 ********************************************************
 * ***在Python代码等效的C++代码上，进行了进一步优化，
 * ***Release编译后，相比Python版，cpu占用略微降低，内存占用降低了大概5倍！，极大的优化了性能（不同电脑环境可能不一样）
 * ***这得益于C++语言本身，以及少量代码方面的优化
 * ***C++代码在原先的基础上进行了一些修改和接口拓展，使用更方便
 * ***修改了构造函数，支持在设计界面中直接使用（提升为...）
 ************************************************************
 * ****请确保Qt大版本为 Qt6
 * ****如使用Qt5，可尝试将enterEvent(QEnterEvent* event)函数的参数类型改为QEvent*，
 * ****并参考注释和leaveEvent函数修改函数内容，以防止报错
 * ****可能还会有其他错误，请自行修改
******************************************************************************/

#include <QApplication>

//可能有多余的不需要的头文件，可自行判断删除（不删也没事）
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QFont>
#include <QTimer>
#include <QEnterEvent>
#include <QEvent>
#include <QCursor>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPen>
#include <QRgb>
#include <QPainterPath>
#include <QtMath>
#include <QPointF>
#include <QRegularExpression>

class mypushbutton : public QFrame
{
private:
    int timeInterval = 10;              //定时器时间间隔，单位：ms
    QTimer* timer = Q_NULLPTR;          //定时器对象
    QPointF mouse_point;                //记录鼠标进入和离开时的坐标
    int max_radius;                     //最大半径
    int radius = 0;                     //绘制半径
    int radius_var = 2;                 //半径每次改变的值（增大或减小）
    QColor fill_color;                  //填充颜色
    int btn_radius = 0;                 //按钮圆角半径

public:
    QPushButton* btn = Q_NULLPTR;       //按钮对象

public:
    mypushbutton(QWidget* parent = Q_NULLPTR);  //构造函数
    void setTimeInterval(int timeinitval);      //设置定时器时间间隔，控制填充速度
    void setText(QString text,
                 QFont font = QFont("微软雅黑"),
                 QColor fontcolor = Qt::white); //设置文本内容
    void setFillColor(QColor fillcolor);        //设置填充颜色
    void resize(const QSize& size);                    //重写父类resize（“重写”这个词在这里可能用的不恰当，但我想不出其他词了）
    void resize(int w,int h);                   //重载版本
    void setStyleSheet(const QString& style);    //重写父类setStyleSheet（“重写”这个词在这里可能用的不恰当）

protected:
    virtual void enterEvent(QEnterEvent* event) override;//重写鼠标进入事件
    virtual void leaveEvent(QEvent*) override;           //重写鼠标离开事件
    virtual void paintEvent(QPaintEvent* event) override;//重写绘图事件

};

mypushbutton::mypushbutton(QWidget* parent)
    :QFrame(parent),timer(new QTimer(this)),
    fill_color(qRgb(255,89,0)),btn(new QPushButton(this))
{
    timer->setInterval(timeInterval);                                   //设置定时器时间间隔
    btn->resize(size());                                                //重设按钮大小与父类一样
    max_radius = qSqrt(width() * width() + height() * height());        //计算最大半径

    static QRegularExpression re("border-radius:\\s*(\\d+)px");         //正则获取圆角半径
    re.match(styleSheet()).hasMatch();
    btn_radius = re.match(styleSheet()).captured(1).toInt();
    //connect(btn,&QPushButton::clicked,this,[]{qDebug()<<"clicked";}); //qDeubg测试按钮功能
}

void mypushbutton::setTimeInterval(int timeinitval)
{
    timer->setInterval(timeinitval);
}

void mypushbutton::setText(QString text,QFont font,QColor fontcolor)
{
    btn->setFont(font);
    btn->setText(text);
    btn->setStyleSheet("color: rgb("+
                       QString::number(fontcolor.red())+","+
                       QString::number(fontcolor.green())+","+
                       QString::number(fontcolor.blue())+")");  //设置字体颜色（此处为狗屎代码，可以自行优化）
}

void mypushbutton::setFillColor(QColor fillcolor)
{
    fill_color = fillcolor;
}

void mypushbutton::resize(const QSize& size)
{
    QFrame::resize(size);           //调用父类resize，设置QFrame大小
    btn->resize(size);              //调用btn的resize，设置按钮大小和QFramer一样
    max_radius = qSqrt(width() * width() + height() * height());    //重新计算最大半径
}

void mypushbutton::resize(int w, int h) //重载版本
{
    QFrame::resize(w,h);
    btn->resize(w,h);
    max_radius = qSqrt(width() * width() + height() * height());
}

void mypushbutton::setStyleSheet(const QString& style)
{
    QFrame::setStyleSheet(style);
    static QRegularExpression re("border-radius:\\s*(\\d+)px");         //正则获取圆角半径
    re.match(styleSheet()).hasMatch();
    btn_radius = re.match(styleSheet()).captured(1).toInt();
}

void mypushbutton::enterEvent(QEnterEvent* event)
{
    mouse_point = event->position();            //记录鼠标进入坐标
    timer->disconnect();                        //断开可能的timer的所有连接
    connect(timer, &QTimer::timeout, this, [=]{ //定时器触发，半径增大
        radius += radius_var;
        if (radius > max_radius)
        {
            timer->stop();                      //达到最大半径，定时器停止
            return;
        }
        update();});                            //更新UI（作用类似刷新显示，（好像？大概是？🫤））
    timer->start();                             //定时器开始
}

void mypushbutton::leaveEvent(QEvent*)
{                                               //类似enterEvent
    mouse_point = mapFromGlobal(QCursor::pos());
    timer->disconnect();
    connect(timer, &QTimer::timeout, this, [=]{ //定时器触发半径减小
        radius -= radius_var;
        if (radius < 0)
        {
            timer->stop();                      //减小到小于0时定时器停止
            return;
        }
        update();});
    timer->start();
}

void mypushbutton::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);
    if (!mouse_point.isNull())
    {
        QPainter painter(this);                             //创建画家对象，绘制窗口为this（也就是QFrame）
        QPainterPath path;                                  //创建绘制路径（区域（可以这么理解？🫤））
        painter.setRenderHint(QPainter::Antialiasing);      //设置抗锯齿
        painter.setBrush(QBrush(fill_color));               //设置填充颜色
        painter.setPen(Qt::NoPen);                          //设置无边框
        path.addRoundedRect(rect(),btn_radius,btn_radius);  //为路径添加圆角矩形区域（也就是QFrame的区域）
        painter.setClipPath(path);                          //设置路径
        painter.drawEllipse(mouse_point,radius,radius);     //画圆
    }
    return;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;

    mypushbutton btn(&w);
    btn.setText("悬浮会变色喔",QFont("微软雅黑",25),Qt::white);
    btn.setFillColor(QColor(255,102,153));
    btn.move(200,400);
    btn.resize(440,140);
    btn.setStyleSheet("*{"
                      "border: none; "
                      "border-radius: 10px;}"

                      "QPushButton{"
                      "background-color: rgba(0, 0, 0, 0);}"

                      "QFrame{"
                      "background-color: rgb(46, 22, 177);}");

    //关于setStyleSheet函数，因为mypushbutton的父类是QFarme，所以为了达到效果，请设置时指定作用的对象
    //如上方所示：设置背景色时请指定为：QFarme{/*...*/}
    //并请确保设置按钮背景色为透明以达到效果，即：QPushButton{background-color: rgba(0, 0, 0, 0);}，最后一位数字为0即可
    //设置圆角时可使用*号一起设置，这样更方便，也可QFrame和QpushButton分别单独设置
    //通用示例：
    /*
        "*{"
        "border: none; "
        "border-radius: 10px;}"//设置圆角半径为10

        "QPushButton{"
        "background-color: rgba(0, 0, 0, 0);}"

        "QFrame{"
        "background-color: rgb(/*在这里填入想要的背景色的RGB值/*);}"
    */


    w.resize(800,800);
    w.setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00bd39, "
                    "stop:0.1 #00b844, stop:0.2 #00b44f, stop:0.3 #00af59, stop:0.4 #00aa64, "
                    "stop:0.5 #01a66f, stop:0.6 #01a17a, stop:0.7 #019c84, stop:0.8 #01988f, "
                    "stop:0.9 #01939a);");
    w.show();
    return a.exec();
}


