#ifndef QWIDGET_FANCYUI_TOGGLESWITCHSTYLE_H
#define QWIDGET_FANCYUI_TOGGLESWITCHSTYLE_H

#include <QProxyStyle>
#include <QColor>

class QVariantAnimation;

namespace fancy
{
    class ToggleSwitch;
    class ControlState;
    class ToggleSwitchIndicator;

    class ToggleSwitchStyle : public QProxyStyle
    {
    public:
        explicit ToggleSwitchStyle(ToggleSwitch *parent);

        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        [[nodiscard]] qreal indicatorOffset() const;

    private:
        void updateTrackColor(const ControlState &state) const;
        ToggleSwitchIndicator* _indicator;
        ToggleSwitch *_toggle;
        mutable QColor _track; // 当前绘制的轨道填充色，动画期间为中间色
        mutable QColor _lastTrack; // 上一次的轨道填充色，用于避免重绘时重复启动动画
        QVariantAnimation *_trackColorAnimation; // 轨道填充颜色过渡动画
    };
} // fancy

#endif // QWIDGET_FANCYUI_TOGGLESWITCHSTYLE_H
