//
// Created by TX on 2025/8/26.
//

#ifndef QWIDGET_FANCYUI_CHECKBOXSTYLE_H
#define QWIDGET_FANCYUI_CHECKBOXSTYLE_H
#include <QCheckBox>
#include <QPainter>
#include <QPainterPath>
#include <QProxyStyle>
#include <QStyleOption>
#include <QVariantAnimation>

#define CHECKBOX_STYLE_PRIVATE_DATA \
private:\
    mutable int _checkbox_side; \
    mutable bool _flag; \
    mutable QPointF _p1; \
    mutable QPointF _p2; \
    int _circum_radius; \
    mutable QPointF _tick_p1; \
    mutable QPointF _tick_p2; \
    QVariantAnimation *_tick_animation_1; \
    QVariantAnimation *_tick_animation_2; \
    QCheckBox* _device;

#define CHECKBOX_STYLE_SET_TICK_P1 \
    void setTickP1(const QVariant &value){_tick_p1 = value.toPointF(); _device->update();}

#define CHECKBOX_STYLE_SET_TICK_P2 \
    void setTickP2(const QVariant &value){_tick_p2 = value.toPointF(); _device->update();}

#define CHECKBOX_STYLE_SER_CIRCUM_RADIUS \
    void setCircumRadius(const QVariant &value){ _circum_radius = value.toReal(); _device->update();}

#define CHECKBOX_STYLE_PRIVATE_SLOTS private slots: \
    CHECKBOX_STYLE_SET_TICK_P1 \
    CHECKBOX_STYLE_SET_TICK_P2 \
    CHECKBOX_STYLE_SER_CIRCUM_RADIUS

#define CHECKBOX_STYLE_STATEMENT(Class) \
public: \
    explicit Class(QCheckBox *parent); \
    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override; \
    int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const override; \
    int indicatorSide() const noexcept { return _checkbox_side; } \
    CHECKBOX_STYLE_PRIVATE_SLOTS \
    CHECKBOX_STYLE_PRIVATE_DATA

namespace fancy
{
    // 正常尺寸，指示器大小：21x21
    class CheckBoxStyle final : public QProxyStyle
    {
        Q_OBJECT
        CHECKBOX_STYLE_STATEMENT(CheckBoxStyle)
    };

    // 小尺寸，指示器大小：16x16
    class CheckBoxStyle_SM final : public QProxyStyle
    {
        Q_OBJECT

        CHECKBOX_STYLE_STATEMENT(CheckBoxStyle_SM)
    };
} // fancy

#endif //QWIDGET_FANCYUI_CHECKBOXSTYLE_H
