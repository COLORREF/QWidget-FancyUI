#ifndef FANCYSTYLE_H
#define FANCYSTYLE_H
#include <QRadioButton>
#include <QStyle>
#include <QStyleOption>
#include <QProxyStyle>
#include <QPalette>
#include <QPainter>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QObject>
#include <QWidget>
#include <QCheckBox>
#include <QAbstractButton>
#include <QtMath>
#include <QQueue>
#include <QColor>
#include <QMap>
#include <QStyleOptionFocusRect>
#include "precompile.h"
#include "FancyAnimation.h"


// RadioButton-text ---- QPalette::ColorRole::WindowText
// CheckBox-text ---- QPalette::ColorRole::WindowText

// Windows10 和 Windows11 运行效果不一致！！！！
// PushButton-text ---- QPalette::ColorRole::NoRole ， NoRole则painter->setPen有效

// CheckBox-indicator ---- QPalette::ColorRole::Highlight，但设置无效
// RadioButton-indicator ---- QPalette::ColorRole::Highlight，但设置无效
// CheckBox-✓ ---- QPalette::ColorRole::HighlightedText

class ControlColors final : public QObject
{
    Q_OBJECT
signals:
    void disEnabledColorChange(const QColor&);
    void normalBorderColorChange(const QColor&);
    void buttonBorderColorChange(const QColor&);
    void buttonBottomLineColorChange(const QColor&);
    void prominenceColorChange(const QColor&);
    void backgroundColorChange(const QColor&);
    void buttonBackgroundColorChange(const QColor&);
    void textColorChange(const QColor&);

private:
    ControlColors();

    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,disEnabled,DisEnabled,emit disEnabledColorChange(_disEnabled);)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,normalBorder,NormalBorder,emit normalBorderColorChange(_normalBorder);)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,buttonBorder,ButtonBorder,emit buttonBorderColorChange(_buttonBorder);)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,buttonBottomLine,ButtonBottomLine,emit buttonBottomLineColorChange(_buttonBorder);)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,buttonBackground,ButtonBackground,emit buttonBackgroundColorChange(_buttonBackground);)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,prominence,Prominence,emit prominenceColorChange(_prominence);)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(QColor,text,Text,emit textColorChange(_text);)
    F_PRIVATE_PROPERTY_PUBLIC_GET_PRIVATE_SET(QColor, theme, Theme,{})

public:
    static ControlColors* controlColors();

    QColor hover(const QColor& color);// 根据原始颜色自动计算合适的鼠标悬浮时的颜色,透明色不适用
    static QColor autoTextColor(const QColor& backgroundColor);// 根据被背景色自动计算合适的文字颜色并返回

    // 反转颜色
    static inline QColor invertColor(const QColor &color){return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());}
    // 背景透明的控件鼠标交互时的颜色
    inline QColor transparent(){return Qt::GlobalColor::transparent;}
    inline QColor transparentHoverLight(){return QColor(200,200,200,50);}
    inline QColor transparentHoverDark(){return QColor(255,255,255,10);}
    inline QColor transparentClickLight(){return QColor(170,170,170,50);}
    inline QColor transparentClickDark(){return QColor(255,255,255,15);}
    QColor transparentHover();
    QColor transparentClick();

};

// 基础的控件状态
struct ControlState
{
    ControlState(QStyle::State state)
    {
        enable = state & QStyle::StateFlag::State_Enabled;
        on = state & QStyle::StateFlag::State_On;
        off = state & QStyle::StateFlag::State_Off;
        over = state & QStyle::StateFlag::State_MouseOver;
        sunken = state & QStyle::StateFlag::State_Sunken;
    }
    bool enable;
    bool on ;
    bool off;
    bool over;
    bool sunken;
};


// 可选中的具有指示器的控件状态（如单选框、复选框）
struct CheckableControlState : ControlState
{
    CheckableControlState(QStyle::State state)
        :ControlState(state)
    {
        normal = enable && off && !over && !sunken;
        normal_over = enable && off && over && !sunken;
        normal_sunke = enable && off && over && sunken;
        selected = enable && on && !over && !sunken;
        select_sunken = enable && on && over && sunken;
        select_over = enable && on && over && !sunken;
        unenable = !enable && off;
        unenable_select = !enable && on;
    }

    // state = 0;// 普通状态
    // state = 5;// 未选中但鼠标悬浮
    // state = 4;// 未选中但鼠标按下
    // state = 1;// 选中状态
    // state = 2;// 选中且鼠标按下
    // state = 3;// 选中但鼠标悬浮
    // state = 6; // 禁用状态
    // state = 7;// 禁用且选中
    // state = 8;// 禁用未选中，但上一次是禁用且选中
    bool normal;
    bool normal_over;
    bool normal_sunke;
    bool selected;
    bool select_sunken ;
    bool select_over;
    bool unenable;
    bool unenable_select;
};

//不可选中控件状态（如默认的 QPushButton）
struct UnCheckableControlState : ControlState
{
    UnCheckableControlState(QStyle::State state)
        :ControlState(state)
    {
        normal = enable && !over && !sunken;
        normal_over = enable && over && !sunken;
        normal_sunke = enable && over && sunken;
        unenable = !enable;
    }
    // state = 0普通状态
    // state = 1鼠标按下
    // state = 2鼠标悬浮
    // state = 3禁用状态
    bool normal;
    bool normal_over;
    bool normal_sunke;
    bool unenable;
};


// 可选中的以widget为基类，没有选中指示器的控件状态（如设置了可选中的 QPushButton）
struct CheckableWidgetState : ControlState
{
    CheckableWidgetState(QStyle::State state)
        :ControlState(state)
    {
        normal = enable && !on && !over && !sunken;
        normal_over = enable && !on && over && !sunken;
        normal_sunke = enable && !on && over && sunken;
        selected = enable && on && !over && !sunken;
        select_sunken = enable && on && over && sunken;
        select_over = enable && on && over && !sunken;
        unenable = !enable && !on;
        unenable_select = !enable && on;
    }
    // state = 0;// 普通状态
    // state = 5;// 未选中但鼠标悬浮
    // state = 4;// 未选中但鼠标按下
    // state = 1;// 选中状态
    // state = 2;// 选中且鼠标按下
    // state = 3;// 选中但鼠标悬浮
    // state = 6; // 禁用状态
    // state = 7;// 禁用且选中
    // state = 8;// 禁用未选中，但上一次是禁用且选中
    bool normal;
    bool normal_over;
    bool normal_sunke;
    bool selected;
    bool select_sunken ;
    bool select_over;
    bool unenable;
    bool unenable_select;
};

class FancyStyleBase : public QProxyStyle
{
    Q_OBJECT
public:
    FancyStyleBase(QStyle *style = nullptr);
    FancyStyleBase(const QString &key);
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
protected:
    ControlColors * _colors;
};

class RadioButtonStyle : public FancyStyleBase
{
    Q_OBJECT
public:
    explicit RadioButtonStyle(QRadioButton *target);
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void enableJumpAnimation(bool isEnable);
private:
    mutable bool _UnEnableOn = false;
    QRadioButton *_target;
    QParallelAnimationGroup* _group;
    SimpleAnimation* _jumpAnimation;
    SimpleAnimation* _indicatorAnimation;
    bool _enableJumpAnimation = false;
};

class CheckBoxStyle : public FancyStyleBase
{
    Q_OBJECT
public:
    explicit CheckBoxStyle(QCheckBox* target);
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
private:
    SimpleAnimation* _animation;
    QCheckBox* _target;
};

class PushButtonStyleBase: public FancyStyleBase
{
    Q_OBJECT
public:
    PushButtonStyleBase(QAbstractButton* target, bool checkable = false);
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    virtual void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void setCheckable(bool checkable);
protected:
    QAbstractButton* _target;
    QLine bottomLine(const QRect& rect,int filletRadius = 0)const;
    virtual void unCheckablePaint(QPainter *painter, qint8 state, const QRect& rect, const QPainterPath& clipPath)const;
    virtual void checkablePaint(QPainter *painter, qint8 state, const QRect& rect, const QPainterPath& clipPath)const;
    bool _checkable;
    int _buttonRadius = 5;
};

class ThemePushButtonSyle : public PushButtonStyleBase
{
    Q_OBJECT
public:
    ThemePushButtonSyle(QAbstractButton* target);
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    virtual void unCheckablePaint(QPainter *painter, qint8 state, const QRect& rect, const QPainterPath& clipPath)const override;
};

class RipplePushButtonStyle : public ThemePushButtonSyle
{
    Q_OBJECT
public:
    RipplePushButtonStyle(QAbstractButton* target);
    virtual bool eventFilter(QObject *obj, QEvent *event)override;
    virtual void unCheckablePaint(QPainter *painter, qint8 state, const QRect& rect, const QPainterPath& clipPath)const override;

    F_PRIVATE_POINTER_PUBLIC_GET(ClickRippleAnimation*,ripple)
private:
    QPointF _mousePoint;
};

class TransparentPushButtonStyle : public PushButtonStyleBase
{
    Q_OBJECT
public:
    TransparentPushButtonStyle(QAbstractButton* target, bool drawBorder = false, bool checkable = false, bool preserveTransparency = false);
    void preserveTransparency(bool preserve);
protected:
    virtual void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled,
                              const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    virtual void unCheckablePaint(QPainter *painter, qint8 state, const QRect& rect, const QPainterPath& clipPath)const override;
    virtual void checkablePaint(QPainter *painter, qint8 state, const QRect& rect, const QPainterPath& clipPath)const override;

private:
    bool _drawBorder;
    bool _preserveTransparency;
};

class ToolButtonStyleBase : public PushButtonStyleBase
{
    Q_OBJECT
public:
    ToolButtonStyleBase(QAbstractButton* target, bool checkable = false);
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class ScrollAreaStyle : public QProxyStyle
{
    Q_OBJECT
public:
    using QProxyStyle::QProxyStyle;
protected:
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
    virtual void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class SliderStyle : public QProxyStyle
{
public:
    SliderStyle(QWidget* target ,QStyle *style = nullptr);
    SliderStyle(QWidget* target, Qt::Orientation orientation, QStyle *style = nullptr);
    void setOrientation(Qt::Orientation orientation);
    virtual bool eventFilter(QObject *obj, QEvent *event)override;
    QWidget* indicator()const;
protected:
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const override;
private:
    class Indicator : public QWidget
    {
        friend class SliderStyle;
    public:
        explicit Indicator(QWidget* parent);
    private:
        Qt::Orientation _orientation = Qt::Orientation::Horizontal;
        SimpleAnimation* _ani = nullptr;
        qreal _r;
        QPointF _center;
        bool _isContains = false;
    protected:
        void enterEvent(QEnterEvent* event)override;
        void leaveEvent(QEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent* event)override;
        void paintEvent(QPaintEvent* )override;
    };
    Indicator* _indicator = nullptr;
    QWidget* _target;
};

#endif // FANCYSTYLE_H
