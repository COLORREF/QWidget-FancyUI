//
// Created by TX on 2025/11/10.
//

#ifndef QWIDGET_FANCYUI_PLAINTEXTEDITSTYLE_H
#define QWIDGET_FANCYUI_PLAINTEXTEDITSTYLE_H
#include <QProxyStyle>

class QVariantAnimation;
class QPlainTextEdit;

namespace fancy
{
    enum class ColorRole;

    class PlainTextEditStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit PlainTextEditStyle(QPlainTextEdit *parent);

        void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        void setRadius(int radius) { _radius = radius; }

        [[nodiscard]] int radius() const { return _radius; }

    protected:
        bool eventFilter(QObject *object, QEvent *event) override;

    private:
        void setIndicator(const QVariant &val);

        void drawBase(QPainter *painter, const QRect &rect, ColorRole fillRole) const;

        int _radius;
        QPlainTextEdit *_parent;
        QLine _indicator;
        QVariantAnimation *_ani;
    };
} // fancy

#endif //QWIDGET_FANCYUI_PLAINTEXTEDITSTYLE_H
