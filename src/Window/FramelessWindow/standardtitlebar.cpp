#include "standardtitlebar.h"

StandardTitleBar::StandardTitleBar(QWidget *parent)
    : SimpleTitleBar{parent},
    _iconLabel{new QLabel(this)},
    _titleLabel{new TitleTextLabel(this)}

{
    this->_titleLabel->setIsClearBeforeNewPaint(false);
    this->_horizontalLayout->setContentsMargins(8, 0, 0, 0);
    this->_iconLabel->setFixedSize(25, 20);
    this->_iconLabel->setScaledContents(true);
    this->_iconLabel->setContentsMargins(0, 0, 5, 0);
    this->_titleLabel->setFont(QFont("Microsoft YaHei", 9));
    this->_horizontalLayout->insertWidget(0, _titleLabel);
    this->_horizontalLayout->insertWidget(0, _iconLabel);

    QPalette p(this->_titleLabel->palette());
    p.setBrush(QPalette::ColorRole::WindowText,Qt::GlobalColor::black);
    this->_titleLabel->setPalette(p);

    // 设置默认图标和标题
    _iconLabel->setPixmap(this->style()->standardIcon(QStyle::StandardPixmap::SP_ComputerIcon).pixmap(20, 20));
    _titleLabel->setText(QApplication::applicationName());

    this->connect(Theme::themeObject(),&Theme::themeChange,this,&StandardTitleBar::onThemeChange);
}

void StandardTitleBar::setIcon(const QPixmap &icon)
{
    this->_iconLabel->setPixmap(icon);
}

void StandardTitleBar::setTitle(const QString &title)
{
    this->_titleLabel->setText(title);
}

void StandardTitleBar::setTitleFont(const QFont &font)
{
    this->_titleLabel->setFont(font);
}

void StandardTitleBar::setTitleColor(const QColor &globalColor)
{
    this->setTitleColor(globalColor, globalColor);
}

void StandardTitleBar::setTitleColor(const QColor &lightColor, const QColor &darkColor)
{
    this->_lightTitltTextColor = lightColor;
    this->_darkTitltTextColor = darkColor;
    this->_titleLabel->update();
}

void StandardTitleBar::onThemeChange(Theme::Type)
{
    this->_titleLabel->update();
}

F_DEFINITION_PAINTEVENT(TitleTextLabel)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::TextAntialiasing);
    if (this->_isClearBeforeNewPaint)
    {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
    }
    if (Theme::isLigth())
        (const_cast<QBrush&>(this->palette().windowText())).setColor(((StandardTitleBar*)this->parentWidget())->_lightTitltTextColor);
    else
        (const_cast<QBrush&>(this->palette().windowText())).setColor(((StandardTitleBar*)this->parentWidget())->_darkTitltTextColor);
    QLabel::paintEvent(event);
}
