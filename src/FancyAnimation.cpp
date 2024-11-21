#include "FancyAnimation.h"
#ifdef _WIN32
ThemeColorManagement::ThemeColorManagement(QObject *parent, const QColor &light, const QColor &dark)
    :QVariantAnimation{parent}
{
    this->_enabled = true;
    this->setDuration(300);
    this->setStartValue(light);
    this->setEndValue(dark);
    this->_runTimeColor = light;
    if(Theme::isDark())
        this->_runTimeColor = dark;
    this->connect(this, &QVariantAnimation::valueChanged, this, &ThemeColorManagement::updateRunTimeColor);
    this->connect(Theme::themeObject(), &Theme::themeChange, this, &ThemeColorManagement::onThemeChange);
}

void ThemeColorManagement::setThemeColor(const QColor &light, const QColor &dark)
{
    this->setStartValue(light);
    this->setEndValue(dark);
    this->_runTimeColor = light;
    if(Theme::isDark())
        this->_runTimeColor = dark;
    emit this->valueChanged(QColor(this->_runTimeColor));
}

QColor ThemeColorManagement::lightColor()
{
    return this->startValue().value<QColor>();
}

QColor ThemeColorManagement::darkColor()
{
    return this->endValue().value<QColor>();
}

bool ThemeColorManagement::isEnable()
{
    return this->_enabled;
}

void ThemeColorManagement::onThemeChange(Theme::Type type)
{
    if(!this->_enabled)
        return;
    if (type == Theme::Type::LIGHT)
        this->setDirection(QAbstractAnimation::Direction::Backward);
    else
        this->setDirection(QAbstractAnimation::Direction::Forward);
    this->start();
}

void ThemeColorManagement::updateRunTimeColor(const QVariant &value)
{
    this->_runTimeColor = value.value<QColor>();
}
#endif
