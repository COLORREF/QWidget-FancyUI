//
// Created by TX on 2025/11/10.
//

#ifndef QWIDGET_FANCYUI_TEXTEDITSTYLE_H
#define QWIDGET_FANCYUI_TEXTEDITSTYLE_H
#include <QProxyStyle>

class QVariantAnimation;
class QTextEdit;

namespace fancy
{
    enum class ColorRole;

    class TextEditStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit TextEditStyle(QTextEdit *parent);

        void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        void setRadius(int radius) { _radius = radius; }

        [[nodiscard]] int radius() const { return _radius; }

    protected:
        bool eventFilter(QObject *object, QEvent *event) override;

    private:
        void setIndicator(const QVariant &val);

        void drawBase(QPainter *painter, const QRect &rect, ColorRole fillRole) const;

        int _radius;
        QTextEdit *_parent;
        QLine _indicator;
        QVariantAnimation *_ani;
    };
} // fancy

#endif //QWIDGET_FANCYUI_TEXTEDITSTYLE_H
