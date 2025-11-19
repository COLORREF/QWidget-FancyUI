//
// Created by TX on 2025/11/7.
//

#ifndef QWIDGET_FANCYUI_LINEEDITSTYLE_H
#define QWIDGET_FANCYUI_LINEEDITSTYLE_H
#include <QProxyStyle>

class QVariantAnimation;
class QLineEdit;

namespace fancy
{
    enum class ColorRole;

    class LineEditStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit LineEditStyle(QLineEdit *parent);

        void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        void setRadius(int radius) { _radius = radius; }

        [[nodiscard]] int radius() const { return _radius; }

    protected:
        bool eventFilter(QObject *object, QEvent *event) override;

    private:
        void setIndicator(const QVariant &val);

        void drawBase(QPainter *painter, const QRect &rect, ColorRole fillRole) const;

        int _radius;
        QLineEdit *_parent;
        QLine _indicator;
        QVariantAnimation *_ani;
    };
} // fancy

#endif //QWIDGET_FANCYUI_LINEEDITSTYLE_H
