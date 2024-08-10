#ifndef RADIOBUTTONSTYLE_H
#define RADIOBUTTONSTYLE_H

#include <QRadioButton>
#include <QStyle>
#include <QStyleOption>
#include <QProxyStyle>
#include <QPalette>
#include <QPainter>
#include "utils/precompile.h"
#include "utils/controlcolors.h"
#include "utils/simpleanimation.h"

class RadioButtonStyle : public QProxyStyle
{
public:
    explicit RadioButtonStyle(QRadioButton *target);
    virtual QRect subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget = nullptr) const override;
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    F_PRIVATE_POINTER_PUBLIC_GET(SimpleAnimation *, animation);
private:
    ControlColors * _controlColors;
    mutable int _radius;
    mutable bool _UnEnableOn = false;
    QRadioButton *_target;
};

#endif // RADIOBUTTONSTYLE_H
