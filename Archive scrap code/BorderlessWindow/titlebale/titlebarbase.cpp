#include "titlebarbase.h"

TitleBarBase::TitleBarBase(QWidget *parent)
    : QWidget{parent},
      close_button{new CloseButton(this)},
      horizontal_layout{new QHBoxLayout(this)},
      horizontal_spacer{new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum)}
{
    this->setFixedHeight(32);
    this->setLayout(this->horizontal_layout);
    QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->horizontal_layout->setSpacing(0);
    this->horizontal_layout->setContentsMargins(0, 0, 0, 0);
    this->horizontal_layout->addItem(this->horizontal_spacer);
    this->horizontal_layout->addWidget(this->close_button);
}

QWidget *TitleBarBase::closeButtontn() const
{
    return this->close_button;
}

QHBoxLayout *TitleBarBase::layout() const
{
    return this->horizontal_layout;
}

void TitleBarBase::mousePressEvent(QMouseEvent *)
{
    this->window()->windowHandle()->startSystemMove();
}
