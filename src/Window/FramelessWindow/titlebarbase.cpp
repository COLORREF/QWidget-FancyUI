#include "titlebarbase.h"

TitleBarBase::TitleBarBase(QWidget *parent)
    : QWidget{parent}
{
    this->_closeButton = new CloseButton(this);
    this->_horizontalLayout = new QHBoxLayout(this);
    this->_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    this->setMinimumHeight(32);
    this->setLayout(this->_horizontalLayout);
    QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->_horizontalLayout->setSpacing(0);
    this->_horizontalLayout->setContentsMargins(0, 0, 0, 0);
    this->_horizontalLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    this->_horizontalLayout->addItem(this->_horizontalSpacer);
    this->_horizontalLayout->addWidget(this->_closeButton);
}

void TitleBarBase::mousePressEvent(QMouseEvent *)
{
    this->window()->windowHandle()->startSystemMove();
}
