//
// Created by TX on 2025/8/26.
//

#include "CheckBoxStyle.h"
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "ControlState.h"
#include "Defs.hpp"
#include "palette.h"


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
#define CHECKBOX_STYLE_INIT(CLASS,indicatorSide,p1x,p1y,p2x,p2y,p3x,p3y,radiusAniTime,p1AniTime,p2AniTime,aniRadiusEnd) \
    QProxyStyle(nullptr), \
    _checkbox_side(indicatorSide), \
    _flag(true), \
    _p1(p1x, p1y), \
    _p2(p2x, p2y), \
    _circum_radius(0), \
    _tick_p1(_p1), \
    _tick_p2(_p2), \
    _tick_animation_1(new QVariantAnimation(this)), \
    _tick_animation_2(new QVariantAnimation(this)), \
    _device(parent) \
{ \
    this->setParent(parent);\
    auto *radius_animation = new QVariantAnimation(this); \
    radius_animation->setDuration(radiusAniTime); \
    radius_animation->setStartValue(0); \
    radius_animation->setEndValue(aniRadiusEnd); \
    _tick_animation_1->setDuration(p1AniTime); \
    _tick_animation_1->setStartValue(_p1); \
    _tick_animation_1->setEndValue(_p1); \
    _tick_animation_2->setDuration(p2AniTime); \
    _tick_animation_2->setStartValue(_p1); \
    _tick_animation_2->setEndValue(QPointF(p3x, p3y)); \
    auto *sequential_animation_group = new QSequentialAnimationGroup(this); \
    auto *parallel_animation_group = new QParallelAnimationGroup(this); \
    sequential_animation_group->addAnimation(_tick_animation_1); \
    sequential_animation_group->addAnimation(_tick_animation_2); \
    parallel_animation_group->addAnimation(radius_animation); \
    parallel_animation_group->addAnimation(sequential_animation_group); \
    auto reverseDirectionAndStart = [parallel_animation_group, this](bool checked) { \
        if (checked) \
        { \
            _tick_animation_1->setDuration(p1AniTime); \
            _tick_animation_2->setDuration(p2AniTime); \
        } \
        else \
        { \
            _tick_animation_1->setDuration(0); \
            _tick_animation_2->setDuration(0); \
        } \
        parallel_animation_group->setDirection(static_cast<QAbstractAnimation::Direction>(!checked)); \
        parallel_animation_group->start(); \
    }; \
    connect(radius_animation, &QVariantAnimation::valueChanged, this, &CLASS::setCircumRadius); \
    connect(_tick_animation_1, &QVariantAnimation::valueChanged, this, &CLASS::setTickP1); \
    connect(_tick_animation_2, &QVariantAnimation::valueChanged, this, &CLASS::setTickP2); \
    connect(parent, &QCheckBox::toggled, radius_animation, reverseDirectionAndStart); \
    connect(&Palette::palette(), &Palette::appThemeChange, parent, QOverload<>::of(&QCheckBox::update)); \
    connect(&Palette::palette(), &Palette::appColorChange, parent, QOverload<>::of(&QCheckBox::update)); \
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
        if (_flag) \
        { \
            _p1.setY(_p1.y() + adjust_Y); \
            _p2.setY(_p2.y() + adjust_Y); \
            _tick_p1 = _p1; \
            _tick_p2 = _p2; \
            _tick_animation_1->setStartValue(_p1); \
            _tick_animation_1->setEndValue(_p2); \
            _tick_animation_2->setStartValue(_p2); \
            _tick_animation_2->setEndValue(QPointF(0.86375 * _checkbox_side, 0.26875 * _checkbox_side + adjust_Y)); \
            _flag = false; \
        } \
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
        if (state._on && _p1 != _tick_p1) \
        { \
            state._enable ? pen.setColor(p[ColorRole::AppAccentText]) : pen.setColor(Qt::GlobalColor::white); \
            pen.setWidthF(1.5); \
            if (state._over) \
                pen.setWidth(2); \
            if (state == VisualState::SelectedPressed) \
                pen.setWidth(1); \
            painter->setPen(pen); \
            painter->drawLine(_p1, _tick_p1); \
            if (_p2 != _tick_p2) \
                painter->drawLine(_p2, _tick_p2); \
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
    CheckBoxStyle::CheckBoxStyle(QCheckBox *parent) :
    CHECKBOX_STYLE_INIT(CheckBoxStyle, 21, 5.01375, 12.20625, 8.95125, 16.8, 18.13875, 5.64375, 150, 130, 260, 15)

    void CheckBoxStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        CHECKBOX_STYLE_drawControl
    }

    int CheckBoxStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        CHECKBOX_STYLE_pixelMetric(21)
    }

    CheckBoxStyle_SM::CheckBoxStyle_SM(QCheckBox *parent) :
    CHECKBOX_STYLE_INIT(CheckBoxStyle_SM, 16, 3.82, 9.3, 6.82, 12.8, 13.82, 4.3, 115, 100, 200, 12)

    void CheckBoxStyle_SM::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        CHECKBOX_STYLE_drawControl
    }

    int CheckBoxStyle_SM::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
    {
        CHECKBOX_STYLE_pixelMetric(16)
    }
} // fancy
