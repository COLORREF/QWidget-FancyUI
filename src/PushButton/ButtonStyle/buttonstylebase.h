#ifndef BUTTONSTYLEBASE_H
#define BUTTONSTYLEBASE_H

#include <QObject>
#include <QProxyStyle>
#include <QWidget>
#include <QAbstractButton>
#include <QPalette>
#include <QStyleOption>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <QQueue>
#include <QParallelAnimationGroup>
#include "utils/precompile.h"

class SimpleAnimation;
class ControlColors;
class ClickRippleAnimation;

class ButtonStyleBase : public QProxyStyle
{
    Q_OBJECT
public:
    ButtonStyleBase(QAbstractButton* target);
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    virtual void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    virtual bool eventFilter(QObject *obj, QEvent *event)override;
    F_PRIVATE_POINTER_PUBLIC_GET(ClickRippleAnimation*,ripple)
private:
    ControlColors * _controlColors;
    QAbstractButton* _target;
    QPointF _mousePoint;
};

#endif // BUTTONSTYLEBASE_H
