#include "ToggleSwitchIndicator.h"

#include <QPainter>
#include <QStyleOptionButton>
#include "ToggleSwitch.h"
#include <QVariantAnimation>

#include "Core/ControlState/ControlState.h"
#include "Core/Defs.h"
#include "Core/Palette/Palette.h"

namespace fancy
{
    namespace
    {
        constexpr qreal KnobTravel = 20;
        constexpr int PositionDuration = 167;
        constexpr int ShapeDuration = 83; // 普通交互形变时长
        constexpr int DisabledShapeDuration = 250; // 禁用形状恢复时长
        constexpr qreal KnobRadius = 7; // 圆与胶囊共用的圆角半径
        constexpr QRectF NormalKnobRect{3.5, 4, 12, 12}; // 普通状态的局部圆形边界
        constexpr QRectF HoveredKnobRect{2.5, 3, 14, 14}; // 悬停状态的局部圆形边界
        constexpr QRectF PressedOffKnobRect{3, 3, 17, 14}; // 关闭时向右伸展的胶囊边界
        constexpr QRectF PressedOnKnobRect{0, 3, 17, 14}; // 开启时向左伸展的胶囊边界
    }

    ToggleSwitchIndicator::ToggleSwitchIndicator(ToggleSwitch* toggle, QObject* parent) :
        QObject(parent),
        _toggle(toggle),
        _knobRect(NormalKnobRect),
        _shapeAnimation(new QVariantAnimation(this)),
        _positionAnimation(new QVariantAnimation(this))
    {
        QEasingCurve easing(QEasingCurve::BezierSpline); // 形变使用的缓动曲线
        easing.addCubicBezierSegment({0, 0}, {0, 1}, {1, 1});
        _shapeAnimation->setEasingCurve(easing);
        _positionAnimation->setEasingCurve(easing);
        _positionAnimation->setDuration(PositionDuration);
        connect(
            _positionAnimation,
            &QVariantAnimation::valueChanged,
            this,
            [this](const QVariant& value) {
                _offset = value.toReal();
                _toggle->update();
            }
        );

        connect(
            _shapeAnimation,
            &QVariantAnimation::valueChanged,
            this,
            [this](const QVariant& value) {
                _knobRect = value.toRectF();
                _toggle->update();
            }
        );
    }

    void ToggleSwitchIndicator::paint(QPainter& painter, const QRectF& track, const QStyleOptionButton& opt)
    {
        const ControlState state(opt.state, ControlType::IndicatorCheckable);
        updateShape(state);
        updatePosition(state);
        const QPointF origin = track.topLeft() + QPointF(_offset, 0);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);

        painter.setBrush(
            Palette::palette()[
                state._enable
                    ? (state._on ? ColorRole::AppAccentText : ColorRole::ToggleSwitchKnobOff)
                    : (state._on ? ColorRole::ToggleSwitchKnobOnDisabled : ColorRole::ToggleSwitchKnobOffDisabled)]
        );
        painter.drawRoundedRect(_knobRect.translated(origin), KnobRadius, KnobRadius);
        painter.restore();
    }

    qreal ToggleSwitchIndicator::offset() const
    {
        // 尚未绘制或存在程序状态变更时，不能返回过期的动画位置
        const bool pending = _completionSerial != _toggle->_completionSerial;
        if (!_initialized
            || (pending && _toggle->_completion == ToggleSwitch::Completion::Cancel)
            || (!pending && _checked != _toggle->isChecked()))
            return _toggle->isChecked() ? KnobTravel : 0.0;
        return _offset;
    }

    void ToggleSwitchIndicator::updateShape(const ControlState& state) const
    {
        QRectF target = NormalKnobRect;
        switch (state)
        {
        case VisualState::Hover:
        case VisualState::SelectedHover:
            target = HoveredKnobRect;
            break;
        case VisualState::Pressed:
        case VisualState::SelectedPressed:
            target = state._on ? PressedOnKnobRect : PressedOffKnobRect;
            break;
        default:
            break;
        }
        if (_shapeAnimation->state() == QAbstractAnimation::Running && _shapeAnimation->endValue().toRectF() == target)
            return;
        _shapeAnimation->stop();
        if (_knobRect == target)
            return;
        _shapeAnimation->setDuration(state._enable ? ShapeDuration : DisabledShapeDuration);
        _shapeAnimation->setStartValue(_knobRect);
        _shapeAnimation->setEndValue(target);
        _shapeAnimation->start();
    }

    void ToggleSwitchIndicator::updatePosition(const ControlState& state)
    {
        const qreal target = state._on ? KnobTravel : 0.0;
        const bool completed = _completionSerial != _toggle->_completionSerial;
        const bool checkedChanged = _checked != state._on;
        _completionSerial = _toggle->_completionSerial;
        _checked = state._on;

        if (!_initialized)
        {
            _initialized = true;
            _offset = target;
            return;
        }

        // 禁用时显示 checked 对应形状
        if (!state._enable)
        {
            _positionAnimation->stop();
            _offset = target;
            return;
        }

        // 按下冻结当前位置；拖拽直接跟随输入，两者都不执行端点动画
        if (_toggle->_mouseState != ToggleSwitch::MouseState::Idle)
        {
            _positionAnimation->stop();
            const bool pressed = state == VisualState::Pressed || state == VisualState::SelectedPressed;
            // 旧交互记录不代表仍处于按压状态，以 option 解析出的视觉状态为准
            _offset = !pressed
                          ? target
                          : (_toggle->_mouseState == ToggleSwitch::MouseState::Dragging
                                 ? _toggle->_dragOffset
                                 : _toggle->_pressOffset);
            return;
        }

        if (completed)
        {
            _positionAnimation->stop();
            if (_toggle->_completion == ToggleSwitch::Completion::Cancel)
            {
                _offset = target;
                return;
            }
            // 最后一次 mouseMove 可能尚未绘制，使用释放时保留的最终坐标
            if (_toggle->_completion == ToggleSwitch::Completion::DragRelease)
                _offset = _toggle->_dragOffset;
            if (_offset != target)
            {
                _positionAnimation->setStartValue(_offset);
                _positionAnimation->setEndValue(target);
                _positionAnimation->start();
            }
        }
        else if (checkedChanged)
        {
            _positionAnimation->stop();
            _offset = target;
        }
        // 普通重绘不重启动画，动画值回调只负责更新当前值和请求重绘
    }
} // fancy
