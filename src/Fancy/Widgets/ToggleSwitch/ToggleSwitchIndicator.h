#ifndef QWIDGET_FANCYUI_TOGGLESWITCHINDICATOR_H
#define QWIDGET_FANCYUI_TOGGLESWITCHINDICATOR_H

#include <QObject>
#include <QRectF>

class QStyleOptionButton;
class QPainter;
class QVariantAnimation;

namespace fancy
{
    class ControlState;
    class ToggleSwitch;

    class ToggleSwitchIndicator : public QObject
    {
    public:
        explicit ToggleSwitchIndicator(ToggleSwitch* toggle, QObject* parent);
        void paint(QPainter& painter, const QRectF& track, const QStyleOptionButton& option);
        [[nodiscard]] qreal offset() const;

    private:
        void updateShape(const ControlState& state) const;
        void updatePosition(const ControlState& state);

        ToggleSwitch* _toggle;
        QRectF _knobRect;
        QVariantAnimation* _shapeAnimation;
        QVariantAnimation* _positionAnimation;
        qreal _offset = 0;
        bool _initialized = false;
        bool _checked = false;
        quint64 _completionSerial = 0;
    };
}
#endif
