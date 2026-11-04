//
// Created by TX on 2025/8/26.
//

#ifndef QWIDGET_FANCYUI_CHECKBOXSTYLE_H
#define QWIDGET_FANCYUI_CHECKBOXSTYLE_H
#include <QPointF>
#include <QProxyStyle>

class QCheckBox;
class QSequentialAnimationGroup;
class QVariantAnimation;

namespace fancy
{
    class CheckBoxStyle final : public QProxyStyle
    {
        Q_OBJECT

    public:
        // 普通指示器为 20x20，小尺寸指示器为 16x16
        explicit CheckBoxStyle(QCheckBox* parent, bool smallIndicatorSize = false);

        void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
        int pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const override;
        void setAnimationEnabled(bool enabled);
        [[nodiscard]] bool isAnimationEnabled() const noexcept { return _animation_enabled; }
        [[nodiscard]] int indicatorSide() const noexcept { return _checkbox_side; }

    private:
        void updateCheckState(Qt::CheckState checkState);
        void syncCheckState();
        void setTickP1(const QVariant& value);
        void setTickP2(const QVariant& value);

        bool _animation_enabled = true;
        int _checkbox_side;
        QPointF _p1;
        QPointF _p2;
        QPointF _tick_p1;
        QPointF _tick_p2;
        QVariantAnimation* _tick_animation_1;
        QVariantAnimation* _tick_animation_2;
        QSequentialAnimationGroup* _tick_animation_group;
        QCheckBox* _device;
    };
} // fancy

#endif //QWIDGET_FANCYUI_CHECKBOXSTYLE_H
