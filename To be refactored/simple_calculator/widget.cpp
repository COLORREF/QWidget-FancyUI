#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget), btnGroup(new QButtonGroup(this))
{
    initUI();       //初始化UI
    initMember();   //初始化成员变量

    //将按钮加入按钮组
    for (auto btn : findChildren<QPushButton*>())   //findChildren模板函数，查找子类，返回list容器
    {
        btnGroup->addButton(btn);                   //遍历按钮，加入按钮组
    }

    //发送者：按钮组，发送信号：当前点击的按钮，接收者：this，槽函数：自定义BtnGroupClicked函数
    connect(btnGroup, &QButtonGroup::buttonClicked, this, &Widget::BtnGroupClicked);


    //连接QSlider和窗体，控制透明度
    connect(ui->TransparencySlider, &QSlider::valueChanged, this, [=](int value) {
        this->setWindowOpacity((static_cast<float>(value) / 100.0)); });

    //连接checkBox_2和窗体，隐藏边框
    connect(ui->checkBox_2, &QCheckBox::clicked, this, [=](bool ischecked) {
        if (ischecked)
        {
            setWindowFlags(Qt::FramelessWindowHint);
        }
        else
        {
            setWindowFlags(Qt::Window);
        }
        show(); });

    //连接checkBox_3和窗体，设置全屏
    connect(ui->checkBox_3, &QCheckBox::clicked, this, [=](bool ischecked) {
        if (ischecked)
        {
            showFullScreen();//全屏
            ui->checkBox_2->setEnabled(false);
            ui->checkBox_4->setEnabled(false);
        }
        else
        {
            showNormal();//恢复默认大小
            ui->checkBox_2->setEnabled(true);
            ui->checkBox_4->setEnabled(true);
        }});

    //连接checkBox_4和窗体，设置置顶
    connect(ui->checkBox_4,&QCheckBox::clicked,this,[=](bool ischecked){
        if(ischecked)
        {
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);//设置窗口为置顶
        }
        else
        {
            setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);//取消置顶
        }
        show(); // 刷新窗口显示
    });
}

Widget::~Widget()
{}

void Widget::initUI()
{
    ui->setupUi(this);//加载UI里的控件

    //全局样式
    QString global = "*{"
                     "border:none;"                                     //去除默认边框和默认的悬浮特效
                     "background-color: rgb(239,244,249);"              //设置背景色
                     "border-radius:15px;}";                            //圆角

    //全局的文本框样式
    QString LineEdit = "QLineEdit{"
                       "background-color: rgb(239,244,249);}";          //设置背景色

    QString LineEditHover = "QLineEdit#lineEdit_show:hover{"
                            "border:1px solid rgba(201,205,209,100);}"; //单独设置鼠标悬停时输入框的边框色

    //全局的按钮样式
    QString Button = "QPushButton{"
                     "background-color: rgba(255,255,255,0);}";         //设置按钮背景色

    QString ButtonHover = "QPushButton:hover{"
                          "background-color: rgba(0,0,0,0);}";          //设置鼠标悬停时按钮背景色

    //等号单独样式
    QString ButtonEqual = "QPushButton#btn_operation_equals_sign{"
                          "background-color: rgba(53,54,87,100);}";     //单独设置等号，背景色

    QString ButtonEqualHover ="QPushButton#btn_operation_equals_sign:hover{"
                               "border:1px solid rgba(0,0,0,200);"      //单独设置等号，鼠标悬停时按钮边框色
                               "background-color: rgba(0,0,0,0);}";     //单独设置等号，鼠标悬停时按钮背景色


    //滑动条样式
    QString Slider = "QSlider::groove{"                                 //选择滑动条的轨道部分
                     "height: 2px;"                                     //设置轨道的高度
                     "background-color: lightgray;}"                    //设置背景颜色

                     "QSlider::handle{"                                 //选择滑动条的滑块部分
                     "background-color: rgb(47, 119, 203);"             //设置背景颜色
                     "width: 8px;"                                      //设置滑块宽度
                     "margin: -9px 0px -9px 0px;"                       //设置滑块的外边距，将滑块居中放置
                     "border-radius: 4px;}"                             //设置滑块圆角

                     "QSlider::handle:hover{"                           //设置滑动条滑块部分悬停时的样式
                     "background-color: rgba(0,0,0,150);}"              //设置背景颜色

                     "QSlider::sub-page{"                               //选择滑动条的已填充部分
                     "background-color: rgb(47,119,203);}";             //设置背景颜色

    //应用上面写好的样式，字符串拼接
    setStyleSheet(global+LineEdit+LineEditHover+Button+ButtonHover+
                  ButtonEqual+ButtonEqualHover+Slider);

    //设置历史输入框的字体颜色
    QScopedPointer<QPalette> historyEdit_palette(new QPalette(ui->lineEdit_history->palette()));    //获取调色板,使用智能指针管理
    historyEdit_palette->setColor(QPalette::Text, qRgb(95, 98, 100));                               //设置字体颜色
    ui->lineEdit_history->setPalette(*historyEdit_palette);                                         //设置调色板

    //设置等号按钮的字体颜色
    ui->btn_operation_equals_sign->setStyleSheet("color:rgb(255,255,255)");                         //设置按钮字体颜色

    //设置按钮的阴影效果
    QGraphicsDropShadowEffect *shadow = Q_NULLPTR;
    for (auto button : findChildren<QPushButton *>())
    {
        shadow = new QGraphicsDropShadowEffect(this);
        shadow->setOffset(0, 0);                        //阴影的偏移量（右，下）
        shadow->setColor(QColor(0, 0, 0));              //阴影的颜色
        shadow->setBlurRadius(15);                      //控制阴影的模糊程度（光源距离）
        button->setGraphicsEffect(shadow);
    }

    ui->frame_2->setBorderRadius(15);                   //设置Frame的圆角大小和文本框一样
    ui->frame_3->setBorderRadius(15);
    ui->frame_2->layout()->setContentsMargins(3,3,3,3); //设置边距即流光粗细
    ui->frame_3->layout()->setContentsMargins(3,3,3,3);
    ui->frame_2->setSpeed(5);
    ui->frame_3->setSpeed(5);
    auto func = [=]()->QColor                           //返回随机颜色
    {
        return QColor(UniformDist_int(0,255,1).at(0),
                      UniformDist_int(0,255,1).at(0),
                      UniformDist_int(0,255,1).at(0), 255);
    };
    ui->frame_2->setDynamicColor(func(),func());
    ui->frame_3->setDynamicColor(func(),func());

    //设置按钮的Frame圆角，填充颜色
    for (auto fra : findChildren<QPushButton*>())
    {
        static_cast<RippleEffect*>(fra->parent())->setRadius(15);//父类转换成子类，否则程序崩溃
        static_cast<RippleEffect*>(fra->parent())->setSpeed(5);
        //static_cast<RippleEffect*>(fra->parent())->setFillColor(func());
    }
}

void Widget::initMember()
{
    //初始化成员变量
    vec_num.resize(2);
    vec_oper.resize(2);
    vec_num[0] = ui->lineEdit_show->text();     //lineEdit_show初始默认0
    vec_num[1] = QString("");
    vec_oper[0] = QString("");
    vec_oper[1] = QString("");
    SpecialOperatorHistory.resize(2);
    SpecialOperatorHistory[0] = vec_num[0];
    SpecialOperatorHistory[1] = vec_num[1];
}

void Widget::clearC()
{
    //清空，全部回归初始状态
    ui->lineEdit_history->clear();
    ui->lineEdit_show->clear();
    ui->lineEdit_show->setText("0");
    vec_num.clear();
    vec_oper.clear();
    initMember();
    pointAilve = false;
    IsDelLeftoperNum = false;
    IsLastResultDel = false;
    IsSpecialOperator = false;
}

void Widget::CheckPoint()
{
    //检查输入框内是否有小数点
    pointAilve = false;                         //状态复位
    for (auto& t : ui->lineEdit_show->text())   //范围for循环，遍历文本框内容
    {
        if (t == '.')
        {
            pointAilve = true;
            break;
        }
    }
}

void Widget::ChooseOperator(QString oper)
{
    //进行非四则运算时
    IsSpecialOperator = false;
    if (vec_oper[0].toString() != "" && vec_num[1].toString() != "" && vec_oper[1].toString() == "=")
    {
        vec_oper[0] = oper;
        vec_num[0] = ui->lineEdit_show->text();
        SpecialOperatorHistory[0] = vec_num[0];
        showHistory(1);
        vec_num[1] = ui->lineEdit_show->text();
        SpecialOperatorHistory[1] = vec_num[1];
        vec_oper[1] = "";
        IsDelLeftoperNum = false;
        return;
    }

    //进行四则运算时
    vec_oper[1] = "";       //先将第二个操作符的等号清除置空
    pointAilve = false;     //点击后，下次输入右操作数，pointAilve复位
    //第一个操作符为空，说明刚输入完左操作数，直接存储操作符
    if (vec_oper[0].toString() == "")
    {
        vec_oper[0] = oper;
        vec_num[1] = vec_num[0];
        SpecialOperatorHistory[1] = vec_num[1];
    }

    //第一个操作符不为空，但为等号
    else if (vec_oper[0].toString() == "=")
    {
        vec_oper[0] = oper;
    }

    //第一个操作符不为空，且右操作数为空，这时候点击操作符表示要更改运算方式
    else if (vec_oper[0].toString() != "" && vec_num[1].toString() == "")
    {
        vec_oper[0] = oper;
    }

    //第一个操作符不为空，且右操作数不为空，直接求出结果，后续如果不输入右操作数只能求一次
    else if (vec_oper[0].toString() != "" && vec_num[1].toString() != "")
    {
        OperLogic1();
        OperLogic2();
        vec_num[1] = QString("");
        SpecialOperatorHistory[1] = vec_num[1];
    }
    showHistory(1);     //每次点击操作符，显示历史记录
}

void Widget::OperLogic1()
{
    //处理四则运算
    if (vec_oper[0].toString() == "+")
        result = vec_num[0].toDouble() + vec_num[1].toDouble();
    else if (vec_oper[0].toString() == "-")
        result = vec_num[0].toDouble() - vec_num[1].toDouble();
    else if (vec_oper[0].toString() == "×")
        result = vec_num[0].toDouble() * vec_num[1].toDouble();
    else if (vec_oper[0].toString() == "÷")
        result = vec_num[0].toDouble() / vec_num[1].toDouble();
    showResult();
}

void Widget::OperLogic2()
{
    //根据按下的操作符按钮更改对应的操作符
    vec_num[0] = formatDouble(result, ui->spinBox->value(), ui->checkBox->isChecked());     //左操作数变为result
    vec_num[1] = QString("");
    SpecialOperatorHistory[0] = vec_num[0];
    SpecialOperatorHistory[1] = vec_num[1];
    IsDelLeftoperNum = false;

    if (BtnText == "+")
        vec_oper[0] = "+";
    else if (BtnText == "-")
        vec_oper[0] = "-";
    else if (BtnText == "×")
        vec_oper[0] = "×";
    else if (BtnText == "÷")
        vec_oper[0] = "÷";
}

QString Widget::formatDouble(double value, int decimalPlaces, bool roundUp)
{//保留小数和四舍五入功能
    QString formattedStr;   
    //四舍五入的情况下，直接使用QString::number函数进行格式化
    if (roundUp)
    {
        formattedStr = QString::number(value, 'f', decimalPlaces);
    }
    //不四舍五入的情况下，使用尾数截断法进行格式化
    else
    {
        formattedStr = QString(std::to_string(value).c_str());
        // 查找小数点的位置
        int dotIndex = formattedStr.indexOf('.');    
        if (dotIndex != -1)
        {
            formattedStr = formattedStr.left(dotIndex + decimalPlaces + 1);//截取小数点及其后的位数，以达到去尾的效果
        }
    }
    //如果结果中没有小数点，添加一个小数点
    if (formattedStr.indexOf('.') == -1 && decimalPlaces > 0)
    {
        formattedStr.append('.');
    }
    //补充不足的小数位数
    while (formattedStr.length() - formattedStr.indexOf('.') - 1 < decimalPlaces)
    {
        formattedStr.append('0');
    }
    return formattedStr;
}

void Widget::showResult()
{
    //显示运算结果
    QString R = formatDouble(result, ui->spinBox->value(), ui->checkBox->isChecked());
    if (!R.isEmpty() && R[R.length() - 1] == '.')
    {
        R.chop(1);
    }
    ui->lineEdit_show->setText(R);
    result = R.toDouble();
}

void Widget::showHistory(int choose)
{
    //根据choose值显示不同的历史记录
    if (!vec_num[0].toString().isEmpty() && vec_num[0].toString()[vec_num[0].toString().length() - 1] == '.')
    {
        QString temp_ = vec_num[0].toString();
        temp_.chop(1);
        vec_num[0] = temp_;
        SpecialOperatorHistory[0] = vec_num[0];
    }
    if (choose == 1)
    {
        ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString() + vec_oper[0].toString());
    }
    else if (choose == 2)
    {   ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString() + vec_oper[0].toString()
                                      + SpecialOperatorHistory[1].toString() + vec_oper[1].toString());
    }
}

void Widget::BtnGroupClicked(QAbstractButton* btn)
{
    BtnText = btn->text();//获取按钮上的文字，判断点击的是哪一个按钮

    //点击数字
    if (BtnText >= "0" && BtnText <= "9" && BtnText.length() == 1)
    {
        if (ui->lineEdit_show->text() == "0" && BtnText != ".") //清除默认的0
        {
            ui->lineEdit_show->clear();
        }
        if (vec_oper[0].toString() == "" || vec_oper[1].toString() == "=")//如果第一个操作符为空，或者上一次运算得出结果后直接点击数字,说明是左操作数
        {
            if (vec_oper[1].toString() == "=" && IsLastResultDel == false)//得出结果后直接点击数字要清除上一次的结果，只能清除一次
            {
                ui->lineEdit_show->clear();
                ui->lineEdit_history->clear();
                IsLastResultDel = true;
            }
            ui->lineEdit_show->insert(BtnText);
            result = ui->lineEdit_show->text().toDouble();
            vec_num[0] = ui->lineEdit_show->text();//左操作数更新
            SpecialOperatorHistory[0] = vec_num[0];
        }
        else//右操作数
        {
            if (IsDelLeftoperNum == false)//显示输入的右操作数前要清除左操作数，只能清除一次
            {
                ui->lineEdit_show->clear();
                IsDelLeftoperNum = true;
            }
            ui->lineEdit_show->insert(BtnText);
            vec_num[1] = ui->lineEdit_show->text();//右操作数更新
            SpecialOperatorHistory[1] = vec_num[1];
        }
    }

    //点击小数点
    else if (BtnText == ".")//小数点不主动存储
    {
        CheckPoint();
        if (!pointAilve)//小数点不存在
        {
            ui->lineEdit_show->insert(BtnText);
            pointAilve = true;
        }
    }

    //点击C清空
    else if (BtnText == "C")
    {
        clearC();//清空全部内容，全部恢复默认
    }

    //退位
    else if (BtnText == "←")//退位，清除一个字符
    {
        //第一个操作符为空，说明是在退位左操作数
        if (vec_oper[0].toString() == "" && !IsSpecialOperator)
        {
            if (ui->lineEdit_show->text().length() == 1)//如果只剩1个字符并且按下了退位，相当于清空
            {
                clearC();
            }
            else//移动光标到前一个字符并删除
            {
                ui->lineEdit_show->setCursorPosition(ui->lineEdit_show->cursorPosition() - 1);
                ui->lineEdit_show->del();
                vec_num[0] = ui->lineEdit_show->text();//重新存储左操作数
                SpecialOperatorHistory[0] = vec_num[0];
            }
        }

        //当第一个操作符不为空，右操作数不为空，且第二个操作符为空，说明是在退位右操作数
        else if (vec_oper[0].toString() != "" && vec_num[1] != "" && vec_oper[1] == "" && !IsSpecialOperator)
        {
            if (ui->lineEdit_show->text().length() == 1)//如果只剩1个字符并且按下了退位
            {
                ui->lineEdit_show->setText("0");
                vec_num[1] = ui->lineEdit_show->text();//存储右操作数为0
                SpecialOperatorHistory[1] = vec_num[1];
            }
            else//移动光标到前一个字符并删除
            {
                ui->lineEdit_show->setCursorPosition(ui->lineEdit_show->cursorPosition() - 1);
                ui->lineEdit_show->del();
                vec_num[1] = ui->lineEdit_show->text();//重新存右操作数
                SpecialOperatorHistory[1] = vec_num[1];
            }
        }

        //当第二个操作数为等号时，只清除历史记录
        if (vec_oper[1] == "=")
        {
            ui->lineEdit_history->clear();
        }

        //第一个操作符为等号时，退位无效

        //当第一个操作符不为空，且右操作数为空时，退位无效

        CheckPoint();//每退位一次，检测是否还有小数点
    }

    //等号
    else if (BtnText == "=")
    {
        IsLastResultDel = false;

        //1、第一个操作符为空，说明刚输入完左操作数，按等号直接显示
        if (vec_oper[0].toString() == "")
        {
            vec_oper[0] = "=";
            result = vec_num[0].toDouble();
            vec_num[1] = vec_num[0];
            SpecialOperatorHistory[1] = vec_num[1];
            showResult();
            showHistory(1);
        }

        //2、第二操作符不是等号的情况下，按等号求值，并且第一个操作符不是等号
        else if (vec_oper[0].toString() != "" && vec_oper[0].toString() != "=" && vec_oper[1] != "=")
        {
            if(vec_oper[0].toString() == "÷" && vec_num[1].toDouble() == 0.0)
            {
                QMessageBox::critical(this, "错误", "The dividend cannot be zero!\n被除数不能为零！",QMessageBox::Ok);
            }
            else
            {
                vec_oper[1] = "=";
                vec_num[1] = ui->lineEdit_show->text();
                showHistory(2);
                OperLogic1();
                SpecialOperatorHistory[1] = vec_num[1];
            }

        }

        //3、持续按等号，右操作数不变
        else if (vec_oper[1].toString() == "=")
        {
            vec_num[0] = formatDouble(result, ui->spinBox->value(), ui->checkBox->isChecked());
            SpecialOperatorHistory[0] = vec_num[0];
            showHistory(2);
            OperLogic1();
        }
    }

    //CE
    else if (BtnText == "CE")
    {
        if (vec_oper[0].toString() == "")//在输入左操作数，等效于按C
        {
            clearC();
        }
        else if (vec_oper[0].toString() != "" && vec_oper[1].toString() == "")//在输入右操作数
        {
            //左操作数保留，第一个操作符保留，右操作数归零，右操作数历史记录更新
            vec_num[1] = QString("0");
            SpecialOperatorHistory[1] = vec_num[1].toString();
            ui->lineEdit_show->setText("0");
        }
        else if (vec_oper[0].toString() != "" && vec_oper[1].toString() == "=")//得出结果后
        {
            //左操作数归零，右操作数和第一个操作符不变，左操作数历史记录更新，result值归零
            vec_num[0] = QString("0");
            SpecialOperatorHistory[0] = vec_num[0].toString();
            result = 0.0;
            ui->lineEdit_show->setText("0");
            ui->lineEdit_history->clear();
        }
    }

    //运算部分
    /*左操作数永远不会为空，默认是0，右操作数默认为空。输入左操作数再点击运算符后，右操作数默认等于左操作数*/
    else if (BtnText == "+")
        ChooseOperator("+");//加
    else if (BtnText == "-")
        ChooseOperator("-"); //减
    else if (BtnText == "×")
        ChooseOperator("×"); //乘
    else if (BtnText == "÷")
        ChooseOperator("÷"); //除

    //倒数
    else if (BtnText == "1/x")
    {
        if (vec_oper[0] == "" && vec_num[0].toDouble() != 0.0)//左操作数//分母不为0
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "1/(" + SpecialOperatorHistory[0].toString() + ")";
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            result = formatDouble(1 / vec_num[0].toDouble(), ui->spinBox->value(), ui->checkBox->isChecked()).toDouble();
            showResult();
            vec_num[0] = formatDouble(result, ui->spinBox->value(), ui->checkBox->isChecked());
        }
        else if (vec_oper[0] != "" && vec_oper[0] != "=" && vec_oper[1] != "=" && vec_num[1].toDouble() != 0.0)//右操作数//分母不为0
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[1] = "1/(" + vec_num[1].toString() + ")";
            vec_num[1] = formatDouble(1 / vec_num[1].toDouble(), ui->spinBox->value(), ui->checkBox->isChecked());
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString() + vec_oper[0].toString() + SpecialOperatorHistory[1].toString());
            ui->lineEdit_show->setText(vec_num[1].toString());
        }
        else if (vec_oper[0] != "" && (vec_oper[1] == "=" || vec_oper[0] == "=") && result != 0.0)//对上次的结果求值//分母不为0
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "1/(" + ui->lineEdit_show->text() + ")";
            vec_num[0]= formatDouble(1 / result, ui->spinBox->value(), ui->checkBox->isChecked());
            result = vec_num[0].toDouble();
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            showResult();
        }
        else//分母为零，提示错误
        {
            QMessageBox::critical(this, "错误", "The denominator cannot be zero!\n分母不能为零！",QMessageBox::Ok);
        }
    }

    //平方
    else if (BtnText == "x²")
    {
        if (vec_oper[0] == "")//左操作数
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "(" + SpecialOperatorHistory[0].toString() + ")²";
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            result = formatDouble(qPow<double, int>(vec_num[0].toDouble(), 2), ui->spinBox->value(), ui->checkBox->isChecked()).toDouble();
            showResult();
            vec_num[0] = formatDouble(result, ui->spinBox->value(), ui->checkBox->isChecked());
        }
        else if (vec_oper[0] != "" && vec_oper[0] != "=" && vec_oper[1] != "=")//右操作数
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[1] = "(" + vec_num[1].toString() + ")²";
            vec_num[1] = formatDouble(qPow<double, int>(vec_num[1].toDouble(), 2), ui->spinBox->value(), ui->checkBox->isChecked());
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString() + vec_oper[0].toString() + SpecialOperatorHistory[1].toString());
            ui->lineEdit_show->setText(vec_num[1].toString());
        }
        else if (vec_oper[0] != "" && (vec_oper[1] == "=" || vec_oper[0] == "=") && result != 0.0)//对上次的结果求值
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "(" + ui->lineEdit_show->text() + ")²";
            vec_num[0] = formatDouble(qPow<double, int>(result, 2), ui->spinBox->value(), ui->checkBox->isChecked());
            result = vec_num[0].toDouble();
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            showResult();
        }
    }

    //开二次方
    else if (BtnText == "√x")
    {
        if (vec_oper[0] == "" && vec_num[0].toDouble() >= 0.0)//左操作数
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "√(" + SpecialOperatorHistory[0].toString() + ")";
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            result = formatDouble(qSqrt<double>(vec_num[0].toDouble()), ui->spinBox->value(), ui->checkBox->isChecked()).toDouble();
            showResult();
            vec_num[0] = formatDouble(result, ui->spinBox->value(), ui->checkBox->isChecked());
        }
        else if (vec_oper[0] != "" && vec_oper[0] != "=" && vec_oper[1] != "=" && vec_num[1].toDouble() >= 0)//右操作数
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[1] = "√(" + vec_num[1].toString() + ")";
            vec_num[1] = formatDouble(qSqrt<double>(vec_num[1].toDouble()), ui->spinBox->value(), ui->checkBox->isChecked());
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString() + vec_oper[0].toString() + SpecialOperatorHistory[1].toString());
            ui->lineEdit_show->setText(vec_num[1].toString());
        }
        else if (vec_oper[0] != "" && (vec_oper[1] == "=" || vec_oper[0] == "=") && result >= 0.0)//对上次的结果求值
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "√(" + ui->lineEdit_show->text() + ")";
            vec_num[0] = formatDouble(qSqrt<double>(result), ui->spinBox->value(), ui->checkBox->isChecked());
            result = vec_num[0].toDouble();
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            showResult();
        }
        else
        {
            QMessageBox::warning(this,
                "错误",
                "Standard calculator does not currently support complex numbers.\n"
                "The square root of a number with an even exponent cannot be a negative number within the real number domain.\n"
                "标准计算器，暂不支持虚数！\n"
                "实数范围内，偶次方根的被开方数不能为负数！",
                QMessageBox::Ok);
        }
    }

    //自然对数
    else if (BtnText == "ln(x)")
    {
        //被操作数不为0
        if (vec_oper[0] == "" &&  vec_num[0].toDouble() > 0.0)//左操作数
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "ln(" + SpecialOperatorHistory[0].toString() + ")";
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            result = formatDouble(log(vec_num[0].toDouble()), ui->spinBox->value(), ui->checkBox->isChecked()).toDouble();
            showResult();
            formatDouble(result, ui->spinBox->value(), ui->checkBox->isChecked());
        }
        else if (vec_oper[0] != "" && vec_oper[0] != "=" && vec_oper[1] != "="  && vec_num[1].toDouble() > 0.0)//右操作数
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[1] = "ln(" + vec_num[1].toString() + ")";
            vec_num[1] = formatDouble(log(vec_num[1].toDouble()), ui->spinBox->value(), ui->checkBox->isChecked());
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString() + vec_oper[0].toString() + SpecialOperatorHistory[1].toString());
            ui->lineEdit_show->setText(vec_num[1].toString());
        }
        else if (vec_oper[0] != "" && (vec_oper[1] == "=" || vec_oper[0] == "=")  && result > 0.0)//对上一次的结果求值
        {
            IsSpecialOperator = true;
            SpecialOperatorHistory[0] = "ln(" + ui->lineEdit_show->text() + ")";
            vec_num[0] = formatDouble(log(result), ui->spinBox->value(), ui->checkBox->isChecked());
            result = vec_num[0].toDouble();
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            showResult();
        }
        else
        {
            QMessageBox::critical(this, "错误",
                "The operand of a logarithmic operation must be greater than zero!\n"
                "对数运算的被操作数必须大于零！",
                QMessageBox::Ok);
        }
    }

    //正负取反
    else if (BtnText == "+/-")
    {
        //对0取反无效//大于0加上负号//小于0删掉负号
        if (vec_oper[0] == "" )//左操作数
        {
            if (vec_num[0].toDouble() > 0.0)
            {
                vec_num[0] = "-" + vec_num[0].toString();
            }
            else if (vec_num[0].toDouble() < 0.0)
            {
                vec_num[0] = vec_num[0].toString().remove(0, 1);
            }
            SpecialOperatorHistory[0] = vec_num[0].toString();
            ui->lineEdit_show->setText(SpecialOperatorHistory[0].toString());
        }
        else if (vec_oper[0] != "" && vec_oper[0] != "=" && vec_oper[1] != "=" )//右操作数
        {
            if (vec_num[1].toDouble() > 0.0)
            {
                vec_num[1] = "-" + vec_num[1].toString();
            }
            else if (vec_num[1].toDouble() < 0.0)
            {
                vec_num[1] = vec_num[1].toString().remove(0, 1);
            }
            SpecialOperatorHistory[1] = vec_num[1].toString();
            ui->lineEdit_show->setText(SpecialOperatorHistory[1].toString());
        }
        else if (vec_oper[0] != "" && (vec_oper[1] == "=" || vec_oper[0] == "="))
        {
            //结果值是double，直接取反
            vec_num[0] = formatDouble(-result, ui->spinBox->value(), ui->checkBox->isChecked());
            SpecialOperatorHistory[0] = vec_num[0].toString();
            result = vec_num[0].toDouble();
            ui->lineEdit_history->setText(SpecialOperatorHistory[0].toString());
            showResult();
        }
    }
}
