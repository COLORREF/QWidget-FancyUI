#include "titlebar.h"
#include "border.h"
#include "borderlesswindow.h"

TitleBar::TitleBar(QWidget *parent)
    : QWidget{parent},
      horizontalLayout{new QHBoxLayout(this)},
      title_label{new QLabel("窗口标题", this)},
      horizontalSpacer{new QSpacerItem(214, 20, QSizePolicy::Expanding, QSizePolicy::Minimum)},
      mini_button{new QPushButton("🗕", this)},
      max_button{new QPushButton("🗖", this)},
      close_button{new QPushButton("🗙", this)}
{
    this->setFixedHeight(28);

    this->horizontalLayout->setSpacing(5);
    this->horizontalLayout->setContentsMargins(10, 0, 10, 0);

    this->horizontalLayout->addWidget(this->title_label);
    this->horizontalLayout->addItem(this->horizontalSpacer);
    this->horizontalLayout->addWidget(this->mini_button);
    this->horizontalLayout->addWidget(this->max_button);
    this->horizontalLayout->addWidget(this->close_button);

    this->mini_button->setFixedSize(35, 20);
    this->max_button->setFixedSize(35, 20);
    this->close_button->setFixedSize(35, 20);

    this->connect(this->mini_button, &QPushButton::clicked, this->window(), &QWidget::showMinimized);
    this->connect(this->max_button, &QPushButton::clicked, this, &TitleBar::MaximizeButtonClicked);
    this->connect(this->close_button, &QPushButton::clicked, this->window(), &QWidget::close);

    this->mini_button->setStyleSheet("QPushButton {"
                                     "    color: black;"
                                     "    border: none;"
                                     "    background-color: rgba(0,0,0,0); "
                                     "    border-radius: 5px"
                                     "}"
                                     "QPushButton:hover {"
                                     "    background-color: rgba(209,214,219,255);"
                                     "}");

    this->max_button->setStyleSheet("QPushButton {"
                                    "    color: black;"
                                    "    border: none;"
                                    "    background-color: rgba(0,0,0,0); "
                                    "    border-radius: 5px"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: rgba(209,214,219,255);"
                                    "}");

    this->close_button->setStyleSheet("QPushButton {"
                                      "    color: black;"
                                      "    border: none;"
                                      "    background-color: rgba(0,0,0,0); "
                                      "    border-radius: 5px"
                                      "}"
                                      "QPushButton:hover {"
                                      "    color: white;"
                                      "    background-color: rgba(196,43,28,255);"
                                      "}");
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::MouseButton::LeftButton)
    {
        if (this->window()->isMaximized())
        {
            this->max_button->setText("🗖");
            for (Border *border : this->window()->findChildren<Border *>())
                if (border->isHidden())
                    border->show();
        }

        this->window()->windowHandle()->startSystemMove();
        this->isMax = false;
    }
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton)
        this->MaximizeButtonClicked();
}

void TitleBar::MaximizeButtonClicked()
{
    if (!this->window()->isMaximized())
    {
        this->max_button->setText("🗗");
        dynamic_cast<BorderlessWindow *>(this->window())->showMaximized();
        this->window()->update();
    }
    else
    {
        this->max_button->setText("🗖");
        dynamic_cast<BorderlessWindow *>(this->window())->showNormal();
        this->window()->update();
    }
    this->isMax = true;
}
