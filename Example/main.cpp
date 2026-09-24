#include <QApplication>
#include <QFont>
#include <QGuiApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication a(argc, argv);

    QFont font = QApplication::font();
    font.setFamilies({"Segoe UI", "Microsoft YaHei"});
    QApplication::setFont(font);

    Widget w;
    w.show();
    return QApplication::exec();
}
