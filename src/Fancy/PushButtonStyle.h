//
// Created by TX on 2025/9/26.
//

#ifndef QWIDGET_FANCYUI_PUSHBUTTONSTYLE_H
#define QWIDGET_FANCYUI_PUSHBUTTONSTYLE_H
#include <QProxyStyle>
#include <QPushButton>

class QAbstractButton;

namespace fancy
{
    class PushButtonStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit PushButtonStyle(QPushButton *parent);

        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

    private:
        int _buttonRadius;
        QPushButton *_parent;
    };
} // fancy

#endif //QWIDGET_FANCYUI_PUSHBUTTONSTYLE_H
