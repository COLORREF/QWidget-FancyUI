//
// Created by TX on 2025/11/18.
//

#ifndef QWIDGET_FANCYUI_HOME_H
#define QWIDGET_FANCYUI_HOME_H
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class Home;
}

QT_END_NAMESPACE

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent);

    ~HomePage() override;

private:
    Ui::Home *_uiHome;;
};


#endif //QWIDGET_FANCYUI_HOME_H
