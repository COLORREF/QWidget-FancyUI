#include <QApplication>

#include "FramelessWindow/theme.h"
#include "widget.h"


// 仅示例程序，为了方便展示，设置全局样式表，src中不包含任何QSS样式表

QString DARK_STYLE = R"(
QGroupBox,QRadioButton,QLineEdit,QCheckBox{
    color:white;
}
QLineEdit{
    background-color:rgb(10,20,47);
    border:1px solid rgb(123,123,123);
})";

QString LIGHT_STYLE = R"(
QGroupBox,QRadioButton,QLineEdit,QCheckBox{
    color:black;
}
QLineEdit{
    background-color:white;
    border:1px solid rgb(123,123,123);
})";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.connect(Theme::themeObject(), &Theme::themeChange, &a, [&a](Theme::Type type){
        if (type == Theme::Type::DARK)
            a.setStyleSheet(DARK_STYLE);
        else
            a.setStyleSheet(LIGHT_STYLE);
    });
    Widget w;
    w.show();
    return a.exec();
}
