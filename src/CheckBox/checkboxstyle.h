#ifndef CHECKBOXSTYLE_H
#define CHECKBOXSTYLE_H

#include <QObject>
#include <QProxyStyle>
#include <QWidget>
#include <QPalette>
#include <QStyleOption>
#include <QPainter>
#include <QCheckBox>
#include <QPainterPath>
#include "utils/precompile.h"
#include "utils/controlcolors.h"
#include "utils/simpleanimation.h"

class CheckBoxStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit CheckBoxStyle(QCheckBox* target);
    virtual QRect subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget = nullptr) const override;
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;

    F_PRIVATE_POINTER_PUBLIC_GET(SimpleAnimation *, animation);
private:
    ControlColors * _controlColors;
    QCheckBox* _target;
    mutable int _radius = 0;
};

#endif // CHECKBOXSTYLE_H
