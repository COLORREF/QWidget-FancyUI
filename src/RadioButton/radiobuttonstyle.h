#ifndef RADIOBUTTONSTYLE_H
#define RADIOBUTTONSTYLE_H

#include <QRadioButton>
#include <QStyle>
#include <QStyleOption>
#include <QProxyStyle>
#include <QPalette>
#include <QPainter>
#include <QPainterPath>
#include <QParallelAnimationGroup>

class ControlColors;
class RadioButtonStyle : public QProxyStyle
{
public:
    explicit RadioButtonStyle(QRadioButton *target);
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    void enableJumpAnimation(bool isEnable);
private:
    ControlColors * _controlColors;
    mutable bool _UnEnableOn = false;
    QRadioButton *_target;   
    QParallelAnimationGroup* _group;
    bool _enableJumpAnimation = false;
};

#endif // RADIOBUTTONSTYLE_H
