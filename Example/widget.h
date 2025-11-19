//
// Created by TX on 2025/8/2.
//

#ifndef WIDGET_H
#define WIDGET_H

#include "StackWindow.h"


class Widget : public fancy::StackWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    ~Widget() override = default;

private:
    void setUpSidebar();

    void setUpTitleBar();

    QWidget *_homePage;
    QWidget *_basicInputPage;
    QWidget *_iconPage;
    QWidget *_palettePage;
};


#endif //WIDGET_H
