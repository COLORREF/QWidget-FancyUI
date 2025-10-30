//
// Created by TX on 2025/10/12.
//

#ifndef QWIDGET_FANCYUI_TEXTLABELSTYLE_H
#define QWIDGET_FANCYUI_TEXTLABELSTYLE_H
#include <QProxyStyle>

class QLabel;

namespace fancy
{
    class TextLabelStyle : public QProxyStyle
    {
    public:
        explicit TextLabelStyle(QLabel *parent);

        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

    private:
        QLabel *_parent;
    };
} // fancy

#endif //QWIDGET_FANCYUI_TEXTLABELSTYLE_H
