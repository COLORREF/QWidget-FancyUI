//
// Created by TX on 2025/8/30.
//

#include "RadioButtonStyle.h"
#include <QStyleOption>
#include <QVariantAnimation>

#include "ControlState.h"
#include "Defs.h"
#include "palette.h"


namespace fancy
{
    constexpr qreal radio_button_indicator_side = 20;
    constexpr qreal radio_button_outer_circle_radius = radio_button_indicator_side / 2;
    constexpr qreal radio_button_inner_circle_radius = radio_button_indicator_side * 3 / 10;

    RadioButtonStyle::RadioButtonStyle(QRadioButton *parent) :
        QProxyStyle(nullptr)
    {
        this->setParent(parent);

        auto inner_anim = new QVariantAnimation(this);
        inner_anim->setStartValue(0.0);
        inner_anim->setEndValue(radio_button_inner_circle_radius);
        inner_anim->setDuration(200);

        auto reverseDirectionAndStart = [inner_anim](bool checked) {
            inner_anim->setDirection(static_cast<QAbstractAnimation::Direction>(!checked));
            inner_anim->start();
        };

        connect(inner_anim, &QVariantAnimation::valueChanged, this, &RadioButtonStyle::setInnerRadius);
        connect(parent, &QRadioButton::toggled, this, reverseDirectionAndStart);
        connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QRadioButton::update));
        connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QRadioButton::update));
    }

    void RadioButtonStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_RadioButton)
        {
            const auto bopt = qstyleoption_cast<const QStyleOptionButton *>(option);
            if (!bopt)
                return;
            const QRect &rect = bopt->rect;
            ControlState state(bopt->state, ControlType::IndicatorCheckable);
            if (state == VisualState::SelectedDisabled)
                _un_enable_on = true;
            if (state == VisualState::Normal)
                _un_enable_on = false;
            if (!state._enable && state._off && _un_enable_on)
                state = VisualState::Last; // 禁用未选中，但上一次是禁用且选中

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);

            QRectF indicator(this->subElementRect(SE_RadioButtonIndicator, option, widget));
            const QPointF center = indicator.center();
            indicator.adjust(0.5, 0.5, -0.5, -0.5); //画笔宽度修正
            QPen pen;
            Palette &pal = Palette::palette();

            // 绘制外圈
            switch (state)
            {
                case VisualState::Normal :
                    pen.setColor(pal[ColorRole::Border]);
                    break;
                case VisualState::Pressed :
                    pen.setColor(pal[ColorRole::AppAccent]);
                    pen.setWidth(2);
                    indicator.adjust(0.5, 0.5, -0.5, -0.5); //画笔宽度修正
                    break;
                case VisualState::Hover : [[fallthrough]];
                case VisualState::Selected : [[fallthrough]];
                case VisualState::SelectedHover : [[fallthrough]];
                case VisualState::SelectedPressed :
                    pen.setColor(pal[ColorRole::AppAccent]);
                    break;
                case VisualState::Disabled : [[fallthrough]];
                case VisualState::SelectedDisabled : [[fallthrough]];
                case VisualState::Last :
                    pen.setColor(pal[ColorRole::DisEnabled]);
                    break;
                default :
                    break;
            }

            painter->setPen(pen);
            painter->drawEllipse(center, radio_button_outer_circle_radius, radio_button_outer_circle_radius);

            // 绘制内圆背景
            painter->setPen(Qt::NoPen);
            painter->setBrush(pal[state._enable ? ColorRole::AppAccent : ColorRole::DisEnabled]);
            if (const qreal r = _inner_radius * 1.6; state._enable && !state._on)
                painter->drawEllipse(center, r, r);
            if (state._on || state == VisualState::Last)
                painter->drawEllipse(center, radio_button_outer_circle_radius, radio_button_outer_circle_radius);

            // 内圆
            switch (qreal r = _inner_radius; state)
            {
                case VisualState::Normal :
                case VisualState::Pressed :
                case VisualState::Hover :
                    goto bk;
                case VisualState::SelectedHover :
                    r++;
                case VisualState::SelectedPressed :
                    r -= 0.5;
                case VisualState::Selected :
                    painter->setBrush(pal[ColorRole::RadioButtonInnerCircle]);
                case VisualState::Disabled :
                    goto bk;
                case VisualState::SelectedDisabled :
                case VisualState::Last :
                    painter->setBrush(pal[ColorRole::RadioButtonInnerCircle]);
                    r = radio_button_inner_circle_radius;
                default :
                bk: painter->drawEllipse(center, r, r);
            }

            pen.setColor(pal[state._enable ? ColorRole::Text : ColorRole::DisEnabledText]);
            pen.setWidth(0);
            painter->setPen(pen);
            const QRect textRect(rect.x() + radio_button_indicator_side + 8, rect.y(), rect.width(), rect.height());
            painter->drawText(textRect, Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignVCenter, bopt->text);
            painter->restore();
            return;
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }

    int RadioButtonStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        switch (metric)
        {
            case PM_RadioButtonLabelSpacing :
                return 10;
            case PM_ExclusiveIndicatorWidth :
            case PM_ExclusiveIndicatorHeight :
                return 21;
            default :
                break;
        }
        return QProxyStyle::pixelMetric(metric, option, widget);
    }

    void RadioButtonStyle::setInnerRadius(const QVariant &value)
    {
        _inner_radius = value.toReal();
        reinterpret_cast<QRadioButton *>(this->parent())->update();
    }
} // fancy
