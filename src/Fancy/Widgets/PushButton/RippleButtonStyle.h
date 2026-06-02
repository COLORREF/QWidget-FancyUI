//
// Created by TX on 2025/10/5.
//

#ifndef QWIDGET_FANCYUI_RIPPLEBUTTONSTYLE_H
#define QWIDGET_FANCYUI_RIPPLEBUTTONSTYLE_H
#include <QProxyStyle>

class QPushButton;

namespace fancy
{
    class RippleAnimation;

    class RippleButtonStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit RippleButtonStyle(QPushButton *parent, int RippleDuration); // msecs

        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        QPushButton *_parent;
        int _buttonRadius;
        RippleAnimation *_ripple;
    };
} // fancy

#endif //QWIDGET_FANCYUI_RIPPLEBUTTONSTYLE_H
