#include "lightanddarkwidget.h"


LightAndDarkWidget::LightAndDarkWidget(QWidget *parent, QColor light, QColor dark)
    :QWidget{parent},
    _themeColorManagement{new ThemeColorManagement(this,light,dark)}
{
    this->_isClearBeforeNewPaint = false;
    this->connect(this->_themeColorManagement,&ThemeColorManagement::valueChanged, this, static_cast<void(QWidget::*)()>(&QWidget::update));
}

void LightAndDarkWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(this->_themeColorManagement->runTimeColor());

    if (this->_isClearBeforeNewPaint)
    {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
    }
    painter.drawRect(this->rect());
}
