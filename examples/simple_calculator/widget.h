#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QDebug>
#include <QString>
#include <QChar>
#include <QPalette>
#include <QVector>
#include <QScopedPointer>
#include <QVariant>
#include <QSlider>
#include <QException>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <qmath.h>
#include <cmath>
#include <iomanip>
#include "DynamicBounding.h"
#include "GlobalFunctions.h"
#include "RippleEffect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

private:
    Ui::Widget* ui;
    QButtonGroup* btnGroup;
    bool pointAilve = false;
    QVector<QVariant> vec_num;//存储操作数
    QVector<QVariant> vec_oper;//存储操作符
    QVector<QVariant> SpecialOperatorHistory;//存储非四则运算时的操作符和操作数
    QString BtnText;//按钮上的文字
    bool IsDelLeftoperNum = false;//显示输入的右操作数前要清除左操作数，只能清除一次的，使用bool控制次数
    bool IsLastResultDel = false;//得出结果后直接点击数字，清除上一次的结果，只能清除一次，使用bool控制次数
    bool IsSpecialOperator = false;//非四则运算时退位无效，用bool记录状态
    double result;//存储运算结果

private:
    void initUI();//初始化UI
    void initMember();//初始化成员变量
    void clearC();//清空
    void CheckPoint();//检查小数点
    void ChooseOperator(QString oper);//选择四则运算方式
    void OperLogic1();//运算逻辑处理函数1
    void OperLogic2();//运算逻辑处理函数2
    QString formatDouble(double value, int decimalPlaces, bool roundUp);//格式化浮点数
    void showResult();//显示结果
    void showHistory(int choose);//显示历史结果

private slots:
    void BtnGroupClicked(QAbstractButton* btn);//按钮组点击后的处理的槽函数
};

#endif // WIDGET_H







