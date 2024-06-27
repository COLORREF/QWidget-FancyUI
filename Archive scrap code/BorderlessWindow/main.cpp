#include <QApplication>
#include <QWidget>
#include "borderlesswindow.h"
#include "ui_form.h"

//跨平台的无边框窗口，仅使用 Qt提供的模块功能
class Widget : public BorderlessWindow
{
public:
    Widget(QWidget * parent = nullptr):
        BorderlessWindow{parent},
        ui{new Ui::Form}
    {
        ui->setupUi(this->qWidgetUseInSetupUi());//ui文件使用SetupUi函数时，参数使用此函数返回值即可
        this->setWindowTitle("Star Rail");

        //允许你使用QPixmap作为图标，QPixmap支持 ico、png、jpge等多种图片文件
        this->setWindowIcon(QPixmap(":/Star Rail.png"));
    }
    ~Widget()
    {
        delete ui;
    }
private:
    Ui::Form * ui;
};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
