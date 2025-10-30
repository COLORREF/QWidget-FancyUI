//
// Created by TX on 2025/9/30.
//

#ifndef QWIDGET_FANCYUI_TRANSPARENTBUTTONSTYLE_H
#define QWIDGET_FANCYUI_TRANSPARENTBUTTONSTYLE_H
#include <QProxyStyle>

class QPushButton;

namespace fancy
{
    class TransparentButtonStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit TransparentButtonStyle(QPushButton *parent);

        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        void setDrawBorder(bool draw) { _drawBorder = draw; };

        void setRadius(int radius) { _buttonRadius = radius; }

    private:
        bool _drawBorder;
        int _buttonRadius;
        QPushButton *_parent;
    };
} // fancy

#endif //QWIDGET_FANCYUI_TRANSPARENTBUTTONSTYLE_H
