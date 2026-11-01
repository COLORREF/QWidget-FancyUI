#include "ToggleSwitchStyle.h"

#include <QPainter>
#include <QStyleOptionButton>
#include "ToggleSwitchIndicator.h"
#include "Core/ControlState/ControlState.h"
#include <QVariantAnimation>

#include "ToggleSwitch.h"
#include "Core/Defs.h"
#include "Core/Palette/Palette.h"

namespace fancy
{
    namespace
    {
        constexpr int TrackColorDuration = 83; // 轨道颜色渐变时长
        constexpr qreal HoverAlphaReduction = 0.1; // 悬停时强调色 Alpha 的减少比例
        constexpr qreal PressedAlphaReduction = 0.2; // 按下时强调色 Alpha 的减少比例
        constexpr qreal TrackRadius = 10; // 轨道外缘的圆角半径
        constexpr qreal TrackBorderWidth = 1; // Off 轨道的描边宽度
        constexpr qreal TrackBorderInset = TrackBorderWidth / 2; // 描边路径相对外缘的内缩量
    }

    ToggleSwitchStyle::ToggleSwitchStyle(ToggleSwitch* parent) :
        QProxyStyle(nullptr),
        _indicator(new ToggleSwitchIndicator(parent, this)),
        _toggle(parent),
        _trackColorAnimation(new QVariantAnimation(this))
    {
        setParent(parent);
        connect(
            _trackColorAnimation,
            &QVariantAnimation::valueChanged,
            this,
            [this](const QVariant& value) {
                _track = value.value<QColor>();
                _toggle->update();
            }
        );
        auto& palette = Palette::palette();
        connect(&palette, &Palette::appThemeChange, parent, QOverload<>::of(&ToggleSwitch::update));
        connect(&palette, &Palette::appColorChange, parent, QOverload<>::of(&ToggleSwitch::update));
    }

    void ToggleSwitchStyle::drawItemText(QPainter* painter, const QRect& rect, int flags, const QPalette& pal, bool enabled, const QString& text, QPalette::ColorRole textRole) const
    {
        Q_UNUSED(pal);
        Q_UNUSED(textRole);
        if (rect.isEmpty() || text.isEmpty())
            return;
        painter->save();
        painter->setPen(Palette::palette()[enabled ? ColorRole::Text : ColorRole::DisEnabledText]);
        painter->setClipRect(rect, Qt::ClipOperation::IntersectClip);
        const QFontMetrics metrics(painter->font());
        painter->drawText(rect, flags, metrics.elidedText(text, Qt::TextElideMode::ElideRight, rect.width()));
        painter->restore();
    }

    void ToggleSwitchStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
    {
        if (element != CE_CheckBox || !_toggle)
            return QProxyStyle::drawControl(element, option, painter, widget);


        const auto* opt = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (!opt)
            return;
        const ControlState state(opt->state, ControlType::IndicatorCheckable);
        updateTrackColor(state);

        auto& palette = Palette::palette();
        const QColor borderColor = state._on ? Qt::GlobalColor::transparent : palette[state._enable ? ColorRole::ToggleSwitchBorderOff : ColorRole::ToggleSwitchFillDisabled];
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        const QRectF track = _toggle->trackRect();

        // 轨道使用颜色动画的当前值
        painter->setPen(Qt::NoPen);
        painter->setBrush(_track);
        painter->drawRoundedRect(track, TrackRadius, TrackRadius);

        // 描边路径内缩半个画笔宽度，保证描边外缘仍与轨道矩形一致
        constexpr qreal borderRadius = TrackRadius - TrackBorderInset;
        painter->setPen(QPen(borderColor, TrackBorderWidth));
        painter->setBrush(Qt::BrushStyle::NoBrush);
        painter->drawRoundedRect(track.adjusted(TrackBorderInset, TrackBorderInset, -TrackBorderInset, -TrackBorderInset), borderRadius, borderRadius);

        // 绘制文本
        constexpr int alignment = Qt::AlignmentFlag::AlignVCenter | Qt::AlignmentFlag::AlignLeft;
        drawItemText(painter, _toggle->textRect(), alignment, option->palette, state._enable, _toggle->displayText(), QPalette::WindowText);

        painter->restore();
        _indicator->paint(*painter, track, *opt);
    }

    qreal ToggleSwitchStyle::indicatorOffset() const
    {
        return _indicator->offset();
    }

    void ToggleSwitchStyle::updateTrackColor(const ControlState& state) const
    {
        // 根据交互状态确定本次绘制的目标颜色
        auto& palette = Palette::palette();
        const bool pressed = state == VisualState::Pressed || state == VisualState::SelectedPressed;
        const bool hovered = state == VisualState::Hover || state == VisualState::SelectedHover;
        QColor targetTrack; // 目标颜色
        if (!state._enable)
            targetTrack = state._on ? palette[ColorRole::ToggleSwitchFillDisabled] : Qt::GlobalColor::transparent;
        else if (state._on)
        {
            targetTrack = palette[ColorRole::AppAccent];
            const qreal reduction = pressed ? PressedAlphaReduction : hovered ? HoverAlphaReduction : 0;
            targetTrack.setAlphaF(targetTrack.alphaF() * (1 - reduction));
        }
        else
            targetTrack = palette[pressed ? ColorRole::ToggleSwitchTrackOffPressed : hovered ? ColorRole::ToggleSwitchTrackOffHover : ColorRole::ToggleSwitchTrackOff];

        // 首次绘制时无有效的初始值，初始化当前色和目标色
        if (!_track.isValid())
        {
            _lastTrack = targetTrack;
            _track = targetTrack;
            return;
        }
        // 动画每帧触发绘制；目标未变时继续原动画，不重置进度
        if (targetTrack == _lastTrack)
            return;
        // 记录新目标，再从当前显示色接续过渡
        _lastTrack = targetTrack;
        _trackColorAnimation->stop();
        _trackColorAnimation->setDuration(TrackColorDuration);
        _trackColorAnimation->setStartValue(_track);
        _trackColorAnimation->setEndValue(targetTrack);
        _trackColorAnimation->start();
    }
} // fancy
