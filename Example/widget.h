//
// Created by TX on 2025/8/2.
//

#ifndef WIDGET_H
#define WIDGET_H

#include "StackWindow.h"


using namespace fancy;

QT_BEGIN_NAMESPACE

namespace Ui
{
    class Home;
    class BasicInput;
}

QT_END_NAMESPACE

class Widget : public StackWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    ~Widget() override;

private:
    void setUpHome();

    void setUpBasicInput();

    void setUpSidebar();

    void setUpTitleBar();

    QWidget *_home;
    QWidget *_basicInput;

    Ui::Home *_uiHome;;
    Ui::BasicInput *_uiBasicInput;
};


#endif //WIDGET_H
