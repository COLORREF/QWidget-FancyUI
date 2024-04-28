#ifndef UI_SET_H
#define UI_SET_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

class Ui_Set
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *parent)
    {
        parent->resize(502, 335);
        gridLayout = new QGridLayout(parent);
        verticalSpacer_2 = new QSpacerItem(20, 141, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(212, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);
        label = new QLabel(parent);
        QFont font;
        font.setPointSize(30);
        label->setFont(font);
        gridLayout->addWidget(label, 1, 1, 1, 1);
        horizontalSpacer_2 = new QSpacerItem(211, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);
        verticalSpacer = new QSpacerItem(20, 141, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        gridLayout->addItem(verticalSpacer, 2, 1, 1, 1);
        label->setText("set");
    }
};

#endif // UI_SET_H
