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
class ParallelAnimationGroupPool;

class ButtonStyleBase : public QProxyStyle
{
    Q_OBJECT
public:
    ButtonStyleBase(QAbstractButton* target);

    virtual QRect subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget = nullptr) const override;
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    virtual void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    virtual bool eventFilter(QObject *obj, QEvent *event)override;

    F_PRIVATE_POINTER_PUBLIC_GET(ParallelAnimationGroupPool*,animationGroupPool)
private:
    ControlColors * _controlColors;
    QAbstractButton* _target;
    QQueue<QPointF> _paintPoint;// 绘制坐标
    QPointF _mousePoint;
    int _maxRadius = 0;
};

#endif // BUTTONSTYLEBASE_H
