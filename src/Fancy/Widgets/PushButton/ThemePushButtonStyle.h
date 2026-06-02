//
// Created by TX on 2025/9/29.
//

#ifndef QWIDGET_FANCYUI_THEMEPUSHBUTTONSTYLE_H
#define QWIDGET_FANCYUI_THEMEPUSHBUTTONSTYLE_H
#include <QProxyStyle>

class QPushButton;

namespace fancy
{
    class ThemeButtonStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit ThemeButtonStyle(QPushButton *parent);

        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        void setRadius(int radius);

        [[nodiscard]] qreal radius() const;

    private:
        int _buttonRadius;
    };
} // fancy

#endif //QWIDGET_FANCYUI_THEMEPUSHBUTTONSTYLE_H
