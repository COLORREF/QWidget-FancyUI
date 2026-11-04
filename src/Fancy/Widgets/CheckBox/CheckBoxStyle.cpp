//
// Created by TX on 2025/8/26.
//

#include "CheckBoxStyle.h"
#include "Core/Defs.h"
#include "Core/ControlState/ControlState.h"
#include "Core/Palette/Palette.h"

#include <QCheckBox>
#include <QPainter>
#include <QSequentialAnimationGroup>
#include <QStyleOption>
#include <QVariantAnimation>

namespace fancy
{
    namespace
    {
        constexpr int normalIndicatorSide = 20; // 普通指示器边长
        constexpr int smallIndicatorSide = 16; // 小尺寸指示器边长
        constexpr qreal smallIndicatorScale = 1.0 * smallIndicatorSide / normalIndicatorSide; // 小尺寸与普通尺寸的边长之比
        constexpr QPointF checkbox_tick_start_ratio(0.26768, 0.52837); // 勾号起点的归一化坐标
        constexpr QPointF checkbox_tick_corner_ratio(0.42344, 0.68413); // 勾号折点的归一化坐标
        constexpr QPointF checkbox_tick_end_ratio(0.73485, 0.36984); // 勾号终点的归一化坐标
        constexpr QPointF checkbox_partial_start_ratio(0.32008, 0.49809); // 半选横线左端的归一化坐标
        constexpr QPointF checkbox_partial_end_ratio(0.68184, 0.49809); // 半选横线右端的归一化坐标
        constexpr qreal tickDurationMs = 280; // 普通尺寸勾线总时长
        constexpr qreal smallTickDurationMs = tickDurationMs * smallIndicatorScale; // 按边长缩放的小尺寸勾线总时长
        constexpr qreal tickFirstSegmentRatio = 1.0 / 3.0; // 第一段勾线占总时长的比例
        constexpr int tickTotalDuration = qRound(tickDurationMs); // 普通尺寸勾线总时长
        constexpr int tickFirstDuration = qRound(tickDurationMs * tickFirstSegmentRatio); // 普通尺寸第一段勾线时长
        constexpr int smallTickTotalDuration = qRound(smallTickDurationMs); // 小尺寸勾线总时长
        constexpr int smallTickFirstDuration = qRound(smallTickDurationMs * tickFirstSegmentRatio); // 小尺寸第一段勾线时长
        constexpr qreal indicatorCornerRadiusRatio = 4.0 / normalIndicatorSide; // 外框圆角半径与边长之比
        constexpr int indicatorBorderWidth = 1; // 常态外框线宽
        constexpr int pressedIndicatorBorderWidth = 2; // 按下时的外框线宽
        constexpr qreal borderInset = indicatorBorderWidth / 2.0; // 常态描边路径的内缩量
        constexpr qreal pressedBorderExtraInset = (pressedIndicatorBorderWidth - indicatorBorderWidth) / 2.0; // 按下时为对齐外沿增加的内缩量
        constexpr qreal markWidth = 1.3; // 常态勾号和横线的线宽
        constexpr qreal hoveredMarkWidth = 1.7; // 悬停时勾号和横线的线宽
        constexpr qreal pressedMarkWidth = 1.0; // 按下时勾号和横线的线宽
        constexpr int labelSpacing = 8; // 文字与指示器的绘制间距
        constexpr int labelExtraSpace = 2; // 尺寸提示中预留的防裁切宽度
    }

    CheckBoxStyle::CheckBoxStyle(QCheckBox* parent, bool smallIndicatorSize) :
        QProxyStyle(nullptr),
        _checkbox_side(smallIndicatorSize ? smallIndicatorSide : normalIndicatorSide),
        _p1(checkbox_tick_start_ratio * _checkbox_side),
        _p2(checkbox_tick_corner_ratio * _checkbox_side),
        _tick_p1(_p1),
        _tick_p2(_p2),
        _tick_animation_1(new QVariantAnimation(this)),
        _tick_animation_2(new QVariantAnimation(this)),
        _tick_animation_group(new QSequentialAnimationGroup(this)),
        _device(parent)
    {
        setParent(parent);
        const int firstDuration = smallIndicatorSize ? smallTickFirstDuration : tickFirstDuration;
        const int totalDuration = smallIndicatorSize ? smallTickTotalDuration : tickTotalDuration;
        _tick_animation_1->setDuration(firstDuration);
        _tick_animation_1->setStartValue(_p1);
        _tick_animation_1->setEndValue(_p2);
        _tick_animation_2->setDuration(totalDuration - firstDuration);
        _tick_animation_2->setStartValue(_p2);
        _tick_animation_2->setEndValue(checkbox_tick_end_ratio * _checkbox_side);
        _tick_animation_group->addAnimation(_tick_animation_1);
        _tick_animation_group->addAnimation(_tick_animation_2);

        connect(_tick_animation_1, &QVariantAnimation::valueChanged, this, &CheckBoxStyle::setTickP1);
        connect(_tick_animation_2, &QVariantAnimation::valueChanged, this, &CheckBoxStyle::setTickP2);
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
        connect(parent, &QCheckBox::checkStateChanged, this, &CheckBoxStyle::updateCheckState);
#else
        connect(
            parent,
            &QCheckBox::stateChanged,
            this,
            [this](int state) {
                updateCheckState(static_cast<Qt::CheckState>(state));
            }
        );
#endif
        syncCheckState();
        connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QCheckBox::update));
        connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QCheckBox::update));
    }

    void CheckBoxStyle::drawControl(
        ControlElement element,
        const QStyleOption* option,
        QPainter* painter,
        const QWidget* widget
    ) const
    {
        if (element == CE_CheckBox)
        {
            const auto bopt = qstyleoption_cast<const QStyleOptionButton*>(option);
            if (!bopt)
                return;
            const QRect& rect = bopt->rect;
            const int adjustY = (rect.height() - _checkbox_side) / 2;
            QRectF indicatorRect(rect.x(), rect.y() + adjustY, _checkbox_side, _checkbox_side);
            indicatorRect.adjust(borderInset, borderInset, -borderInset, -borderInset);
            const ControlState state(bopt->state, ControlType::IndicatorCheckable);
            Palette& p = Palette::palette();
            QPen pen;
            pen.setWidth(indicatorBorderWidth);
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            switch (state)
            {
            case VisualState::Normal:
                pen.setColor(p[ColorRole::Border]);
                break;
            case VisualState::Pressed:
                pen.setColor(p[ColorRole::AppAccent]);
                pen.setWidth(pressedIndicatorBorderWidth);
                indicatorRect.adjust(
                    pressedBorderExtraInset,
                    pressedBorderExtraInset,
                    -pressedBorderExtraInset,
                    -pressedBorderExtraInset
                );
                break;
            case VisualState::Hover: [[fallthrough]];
            case VisualState::Selected: [[fallthrough]];
            case VisualState::SelectedHover: [[fallthrough]];
            case VisualState::SelectedPressed:
                pen.setColor(p[ColorRole::AppAccent]);
                break;
            case VisualState::Disabled: [[fallthrough]];
            case VisualState::SelectedDisabled:
                pen.setColor(p[ColorRole::DisEnabled]);
                break;
            default:
                painter->restore();
                return;
            }
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            if (state._on || state._partial)
                painter->setBrush(p[state._enable ? ColorRole::AppAccent : ColorRole::DisEnabled]);
            const qreal cornerRadius = _checkbox_side * indicatorCornerRadiusRatio;
            painter->drawRoundedRect(indicatorRect, cornerRadius, cornerRadius);
            if (state._on || state._partial)
            {
                state._enable ? pen.setColor(p[ColorRole::AppAccentText]) : pen.setColor(Qt::GlobalColor::white);
                pen.setWidthF(markWidth);
                if (state._over)
                    pen.setWidthF(hoveredMarkWidth);
                if (state == VisualState::SelectedPressed)
                    pen.setWidthF(pressedMarkWidth);
                painter->setPen(pen);
                const QPointF offset(rect.x(), rect.y() + adjustY);
                if (state._partial)
                {
                    const QPointF start = checkbox_partial_start_ratio * _checkbox_side;
                    const QPointF end = checkbox_partial_end_ratio * _checkbox_side;
                    painter->drawLine(start + offset, end + offset);
                }
                else if (state._on && _p1 != _tick_p1)
                {
                    pen.setCapStyle(Qt::RoundCap);
                    pen.setJoinStyle(Qt::RoundJoin);
                    painter->setPen(pen);
                    if (_p2 != _tick_p2)
                    {
                        const QPointF points[] = {_p1 + offset, _tick_p1 + offset, _tick_p2 + offset};
                        painter->drawPolyline(points, 3);
                    }
                    else
                    {
                        painter->drawLine(_p1 + offset, _tick_p1 + offset);
                    }
                }
            }
            pen.setColor(p[state._enable ? ColorRole::Text : ColorRole::DisEnabledText]);
            pen.setWidth(0);
            painter->setPen(pen);
            const QRect textRect(rect.x() + _checkbox_side + labelSpacing, rect.y(), rect.width(), rect.height());
            painter->drawText(textRect, Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignVCenter, bopt->text);
            painter->restore();
            return;
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }

    int CheckBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
    {
        switch (metric)
        {
        case PM_CheckBoxLabelSpacing:
            return labelSpacing + labelExtraSpace;
        case PM_IndicatorWidth: [[fallthrough]];
        case PM_IndicatorHeight:
            return _checkbox_side;
        default:
            break;
        }
        return QProxyStyle::pixelMetric(metric, option, widget);
    }

    void CheckBoxStyle::setAnimationEnabled(bool enabled)
    {
        if (_animation_enabled == enabled)
            return;
        _animation_enabled = enabled;
        if (!enabled)
            syncCheckState();
    }

    void CheckBoxStyle::updateCheckState(Qt::CheckState checkState)
    {
        if (!_animation_enabled || checkState != Qt::Checked)
        {
            syncCheckState();
            return;
        }
        _tick_animation_group->stop();
        _tick_animation_1->setCurrentTime(0);
        _tick_animation_2->setCurrentTime(0);
        _tick_p1 = _p1;
        _tick_p2 = _p2;
        _tick_animation_group->start();
        _device->update();
    }

    void CheckBoxStyle::syncCheckState()
    {
        _tick_animation_group->stop();
        const bool checked = _device->checkState() == Qt::Checked;
        _tick_p1 = checked ? _p2 : _p1;
        _tick_p2 = checked ? _tick_animation_2->endValue().toPointF() : _p2;
        _device->update();
    }

    void CheckBoxStyle::setTickP1(const QVariant& value)
    {
        _tick_p1 = value.toPointF();
        _device->update();
    }

    void CheckBoxStyle::setTickP2(const QVariant& value)
    {
        _tick_p2 = value.toPointF();
        _device->update();
    }
} // fancy
