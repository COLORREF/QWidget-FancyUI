#include "mswindow.h"
#include "titlebarbase.h"

MSWindow::MSWindow(QWidget *parent, WindowEffectType effectType):
    TransparentEffectWindowBase(parent, effectType)
{
    QWidget* parent_master_scope = TransparentEffectWindowBase::clientArea();
    this->_horizontalLayout = new QHBoxLayout(parent_master_scope);
    this->_clientArea = new MSFilletedCornerWidget(parent_master_scope);
    this->_sidebar = new QWidget(parent_master_scope);
    this->_titleBar->setFixedHeight(50);
    this->_horizontalLayout->setSpacing(0);
    this->_horizontalLayout->setContentsMargins(0, 1, 0, 0);
    this->_horizontalLayout->addWidget(this->_sidebar);
    this->_horizontalLayout->addWidget(this->_clientArea);
    this->_sidebar->setFixedWidth(75);
}

void MSFilletedCornerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(this->_themeColorManagement->runTimeColor());
    QRect rect = this->rect();
    QPainterPath path;
    path.moveTo(0,10);
    path.lineTo(rect.bottomLeft());
    path.lineTo(rect.bottomRight());
    path.lineTo(rect.topRight());
    path.lineTo(10,0);
    path.arcTo(QRect(rect.left(), rect.top(), 20, 20), 90, 90);
    path.closeSubpath();
    if(Theme::isLigth())
        painter.setPen(QPen(QColor(0, 0, 0, 17),1));
    else
        painter.setPen(QPen(QColor(0, 0, 0, 46),1));
    painter.drawPath(path);
}
