//
// Created by TX on 2025/8/26.
//

#include "CheckBoxStyle.h"
#include <QSequentialAnimationGroup>
#include "Core/Defs.h"
#include "Core/ControlState/ControlState.h"
#include "Core/Palette/Palette.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
#define CHECKBOX_STATE_CHANGED &QCheckBox::checkStateChanged
#else
#define CHECKBOX_STATE_CHANGED &QCheckBox::stateChanged
#endif

/**
 *
 * @param CLASS 对象作用域
 * @param indicatorSide 指示器边长
 * @param p1x 勾第一个点的x坐标 计算公式：p1x = 0.23875 * indicatorSide
 * @param p1y 勾第一个点的y坐标 计算公式：p1y = 0.58125 * indicatorSide
 * @param p2x 勾第二个点的x坐标 计算公式：p2x = 0.42625 * indicatorSide
 * @param p2y 勾第二个点的y坐标 计算公式：p2y = 0.8 * indicatorSide
 * @param p3x 勾第三个点的x坐标 计算公式：p3x = 0.86375 * indicatorSide
 * @param p3y 勾第三个点的y坐标 计算公式：p3y = 0.26875 * indicatorSide
 * @param radiusAniTime  选中填充动画运行时间，单位：ms
 * @param p1AniTime 勾选动画第一段时间，单位：ms
 * @param p2AniTime 勾选动画第二段时间，单位：ms
 * @param partialAniTime 半选横线动画时间，单位：ms
 * @param aniRadiusEnd 选中填充动画的结束值（半径动画的半径最大值）即选中方框外接圆半径
 *                  __________________
 *                  |                |
 *                  |             /<-|-------p3
 *        p1 -------|-->\       /    |
 *                  |    \    /      |
 *         p2-------|---->\ /        |
 *                  |________________|
 *
 */
#define CHECKBOX_STYLE_INIT(CLASS,indicatorSide,p1x,p1y,p2x,p2y,p3x,p3y,radiusAniTime,p1AniTime,p2AniTime,partialAniTime,aniRadiusEnd) \
    QProxyStyle(nullptr), \
    _fill_radius(aniRadiusEnd), \
    _checkbox_side(indicatorSide), \
    _p1(p1x, p1y), \
    _p2(p2x, p2y), \
    _circum_radius(0), \
    _tick_p1(_p1), \
    _tick_p2(_p2), \
    _partial_progress(0.0), \
    _tick_animation_1(new QVariantAnimation(this)), \
    _tick_animation_2(new QVariantAnimation(this)), \
    _partial_animation(new QVariantAnimation(this)), \
    _radius_animation(new QVariantAnimation(this)), \
    _tick_animation_group(new QSequentialAnimationGroup(this)), \
    _device(parent) \
{ \
    this->setParent(parent);\
    _radius_animation->setDuration(radiusAniTime); \
    _radius_animation->setStartValue(0); \
    _radius_animation->setEndValue(aniRadiusEnd); \
    _tick_animation_1->setDuration(p1AniTime); \
    _tick_animation_1->setStartValue(_p1); \
    _tick_animation_1->setEndValue(_p2); \
    _tick_animation_2->setDuration(p2AniTime); \
    _tick_animation_2->setStartValue(_p2); \
    _tick_animation_2->setEndValue(QPointF(p3x, p3y)); \
    _tick_animation_group->addAnimation(_tick_animation_1); \
    _tick_animation_group->addAnimation(_tick_animation_2); \
    _partial_animation->setDuration(partialAniTime); \
    _partial_animation->setStartValue(0.0); \
    _partial_animation->setEndValue(1.0); \
    connect(_radius_animation, &QVariantAnimation::valueChanged, this, &CLASS::setCircumRadius); \
    connect(_tick_animation_1, &QVariantAnimation::valueChanged, this, &CLASS::setTickP1); \
    connect(_tick_animation_2, &QVariantAnimation::valueChanged, this, &CLASS::setTickP2); \
    connect(_partial_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) { \
        _partial_progress = value.toReal(); \
        _device->update(); \
    }); \
    auto updateCheckState = [this](int checkState) { \
        if (!_animation_enabled) \
        { \
            syncCheckState(); \
            return; \
        } \
        _tick_animation_group->stop(); \
        _partial_animation->stop(); \
        _tick_animation_1->setCurrentTime(0); \
        _tick_animation_2->setCurrentTime(0); \
        _partial_animation->setCurrentTime(0); \
        _tick_p1 = _p1; \
        _tick_p2 = _p2; \
        _partial_progress = 0.0; \
        _radius_animation->stop(); \
        _radius_animation->setStartValue(_circum_radius); \
        _radius_animation->setEndValue(checkState == Qt::Unchecked ? 0.0 : qreal(aniRadiusEnd)); \
        _radius_animation->start(); \
        if (checkState == Qt::Checked) \
            _tick_animation_group->start(); \
        else if (checkState == Qt::PartiallyChecked) \
            _partial_animation->start(); \
        _device->update(); \
    }; \
    connect(parent, CHECKBOX_STATE_CHANGED, this, updateCheckState); \
    syncCheckState(); \
    connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QCheckBox::update)); \
    connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QCheckBox::update)); \
}
#define CHECKBOX_STYLE_ANIMATION_METHODS(CLASS) \
    void CLASS::setAnimationEnabled(bool enabled) \
    { \
        if (_animation_enabled == enabled) \
            return; \
        _animation_enabled = enabled; \
        if (!enabled) \
            syncCheckState(); \
    } \
    void CLASS::syncCheckState() \
    { \
        _radius_animation->stop(); \
        _tick_animation_group->stop(); \
        _partial_animation->stop(); \
        const auto state = _device->checkState(); \
        _circum_radius = state == Qt::Unchecked ? 0.0 : _fill_radius; \
        _tick_p1 = state == Qt::Checked ? _p2 : _p1; \
        _tick_p2 = state == Qt::Checked ? _tick_animation_2->endValue().toPointF() : _p2; \
        _partial_progress = state == Qt::PartiallyChecked ? 1.0 : 0.0; \
        _device->update(); \
    }

#define CHECKBOX_STYLE_drawControl \
    if (element == CE_CheckBox) \
    { \
        const auto bopt = qstyleoption_cast<const QStyleOptionButton *>(option); \
        if (!bopt) \
            return; \
        constexpr int RADIUS = 2; \
        const QRect &rect = bopt->rect; \
        const int adjust_Y = (rect.height() - _checkbox_side) / 2; \
        QRectF indicatorRect(rect.x(), rect.y() + adjust_Y, _checkbox_side, _checkbox_side); \
        indicatorRect.adjust(0.5, 0.5, -0.5, -0.5); \
        const QPointF center = indicatorRect.center() ;\
        const ControlState state(bopt->state, ControlType::IndicatorCheckable); \
        Palette &p = Palette::palette(); \
        QPen pen; \
        painter->save(); \
        painter->setRenderHint(QPainter::Antialiasing, true); \
        switch (state) \
        { \
            case VisualState::Normal : \
                pen.setColor(p[ColorRole::Border]); \
                break; \
            case VisualState::Pressed : \
                pen.setColor(p[ColorRole::AppAccent]); \
                pen.setWidth(2); \
                indicatorRect.adjust(0.5, 0.5, -0.5, -0.5); \
                break; \
            case VisualState::Hover : [[fallthrough]]; \
            case VisualState::Selected : [[fallthrough]]; \
            case VisualState::SelectedHover : [[fallthrough]]; \
            case VisualState::SelectedPressed : \
                pen.setColor(p[ColorRole::AppAccent]); \
                break; \
            case VisualState::Disabled : [[fallthrough]]; \
            case VisualState::SelectedDisabled : \
                pen.setColor(p[ColorRole::DisEnabled]); \
                break; \
            default : \
                painter->restore(); \
                return; \
        } \
        painter->setPen(pen); \
        painter->drawRoundedRect(indicatorRect, RADIUS, RADIUS); \
        QPainterPath clipPath; \
        clipPath.addRoundedRect(indicatorRect, RADIUS, RADIUS); \
        painter->setClipPath(clipPath); \
        painter->setBrush(p[ColorRole::AppAccent]); \
        if (!state._enable) \
            painter->setBrush(p[ColorRole::DisEnabled]); \
        painter->setPen(Qt::NoPen); \
        painter->drawEllipse(center, _circum_radius, _circum_radius); \
        if (state._on || state._partial) \
        { \
            state._enable ? pen.setColor(p[ColorRole::AppAccentText]) : pen.setColor(Qt::GlobalColor::white); \
            pen.setWidthF(1.5); \
            if (state._over) \
                pen.setWidth(2); \
            if (state == VisualState::SelectedPressed) \
                pen.setWidth(1); \
            painter->setPen(pen); \
            const QPointF offset(rect.x(), rect.y() + adjust_Y); \
            if (state._partial && _partial_progress > 0.0) \
            { \
                const QPointF start(_checkbox_side * 0.25, _checkbox_side * 0.5); \
                const QPointF end(start.x() + _checkbox_side * 0.5 * _partial_progress, start.y()); \
                painter->drawLine(start + offset, end + offset); \
            } \
            else if (state._on && !state._partial && _p1 != _tick_p1) \
            { \
                painter->drawLine(_p1 + offset, _tick_p1 + offset); \
                if (_p2 != _tick_p2) \
                    painter->drawLine(_p2 + offset, _tick_p2 + offset); \
            } \
        } \
        pen.setColor(p[state._enable ? ColorRole::Text : ColorRole::DisEnabledText]); \
        pen.setWidth(0); \
        painter->setPen(pen); \
        painter->setClipping(false); \
        const QRect textRect(rect.x() + _checkbox_side + 8, rect.y(), rect.width(), rect.height()); \
        painter->drawText(textRect, Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignVCenter, bopt->text); \
        painter->restore(); \
        return; \
    } \
    QProxyStyle::drawControl(element, option, painter, widget);
#define CHECKBOX_STYLE_pixelMetric(indicatorSide) \
switch (metric) \
{ \
    case PM_CheckBoxLabelSpacing : \
        return 10; /* 实际使用8px，预留2px防止文字绘制超出边界*/ \
    case PM_IndicatorWidth : [[fallthrough]]; \
    case PM_IndicatorHeight : \
        return indicatorSide; \
    default : \
        break; \
} \
return QProxyStyle::pixelMetric(metric, option, widget);

namespace fancy
{
    namespace
    {
        // 指示器几何参数，勾选坐标相对于指示器左上角，单位：px
        constexpr int checkbox_indicator_side = 21;
        constexpr QPointF checkbox_tick_start(5.01375, 12.20625);
        constexpr QPointF checkbox_tick_corner(8.95125, 16.8);
        constexpr QPointF checkbox_tick_end(18.13875, 5.64375);
        constexpr int checkbox_fill_radius = 15;

        constexpr int small_checkbox_indicator_side = 16;
        constexpr QPointF small_checkbox_tick_start(3.82, 9.3);
        constexpr QPointF small_checkbox_tick_corner(6.82, 12.8);
        constexpr QPointF small_checkbox_tick_end(13.82, 4.3);
        constexpr int small_checkbox_fill_radius = 12;

        // 动画时长，单位：ms
        constexpr int checkbox_radius_animation_duration = 150;
        constexpr int checkbox_tick_animation_1_duration = 115;
        constexpr int checkbox_tick_animation_2_duration = 230;
        constexpr int checkbox_partial_animation_duration = 345;

        constexpr int small_checkbox_radius_animation_duration = 115;
        constexpr int small_checkbox_tick_animation_1_duration = 100;
        constexpr int small_checkbox_tick_animation_2_duration = 200;
        constexpr int small_checkbox_partial_animation_duration = 115;
    }

    CHECKBOX_STYLE_ANIMATION_METHODS(CheckBoxStyle)
    CHECKBOX_STYLE_ANIMATION_METHODS(CheckBoxStyle_SM)

    CheckBoxStyle::CheckBoxStyle(QCheckBox *parent) :
    CHECKBOX_STYLE_INIT(
            CheckBoxStyle,
            checkbox_indicator_side,
            checkbox_tick_start.x(),
            checkbox_tick_start.y(),
            checkbox_tick_corner.x(),
            checkbox_tick_corner.y(),
            checkbox_tick_end.x(),
            checkbox_tick_end.y(),
            checkbox_radius_animation_duration,
            checkbox_tick_animation_1_duration,
            checkbox_tick_animation_2_duration,
            checkbox_partial_animation_duration,
            checkbox_fill_radius)

    void CheckBoxStyle::drawControl(
        ControlElement element,
        const QStyleOption *option,
        QPainter *painter,
        const QWidget *widget) const
    {
        CHECKBOX_STYLE_drawControl
    }

    int CheckBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        CHECKBOX_STYLE_pixelMetric(checkbox_indicator_side)
    }

    CheckBoxStyle_SM::CheckBoxStyle_SM(QCheckBox *parent) :
    CHECKBOX_STYLE_INIT(
            CheckBoxStyle_SM,
            small_checkbox_indicator_side,
            small_checkbox_tick_start.x(),
            small_checkbox_tick_start.y(),
            small_checkbox_tick_corner.x(),
            small_checkbox_tick_corner.y(),
            small_checkbox_tick_end.x(),
            small_checkbox_tick_end.y(),
            small_checkbox_radius_animation_duration,
            small_checkbox_tick_animation_1_duration,
            small_checkbox_tick_animation_2_duration,
            small_checkbox_partial_animation_duration,
            small_checkbox_fill_radius)

    void CheckBoxStyle_SM::drawControl(
        ControlElement element,
        const QStyleOption *option,
        QPainter *painter,
        const QWidget *widget) const
    {
        CHECKBOX_STYLE_drawControl
    }

    int CheckBoxStyle_SM::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        CHECKBOX_STYLE_pixelMetric(small_checkbox_indicator_side)
    }
} // fancy
