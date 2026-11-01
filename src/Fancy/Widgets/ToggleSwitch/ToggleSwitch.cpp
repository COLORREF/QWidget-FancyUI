#include "ToggleSwitch.h"

#include <QEnterEvent>
#include <QFontMetricsF>
#include <QHideEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionButton>
#include <QtMath>

#include "ToggleSwitchStyle.h"

namespace fancy
{
    namespace
    {
        constexpr int TrackWidth = 40; // 轨道固定宽度
        constexpr int TrackHeight = 20; // 轨道固定高度，也是内容行的最小内容高度
        constexpr int HorizontalMargin = 4; // 内容行左、右两侧各自预留的边距
        constexpr int VerticalMargin = 10; // 内容行上、下两侧各自预留的边距
        constexpr int HorizontalPadding = HorizontalMargin * 2; // 左右边距总宽度
        constexpr int VerticalPadding = VerticalMargin * 2; // 上下边距总高度
        constexpr int TextSpacing = 12; // 轨道右缘与状态文字之间的距离
        constexpr int TextOffset = TrackWidth + TextSpacing; // 文字左缘相对内容行左缘的偏移
        constexpr int MinimumWidth = TrackWidth + HorizontalPadding; // 保证完整轨道及左右边距的宽度
        constexpr int PreferredTextWidth = 154; // 有文字时控件的默认建议宽度，长文字可继续扩展
        constexpr int DefaultFontPixelSize = 14; // 状态文字的默认字体像素大小
        constexpr int DragThreshold = 1; // 从按下位置水平移动至少此距离后触发拖拽，单位为逻辑像素
        constexpr qreal KnobTravel = 20; // 指示器水平行程，单位为逻辑像素
    }

    ToggleSwitch::ToggleSwitch(QWidget* parent) :
        QAbstractButton(parent),
        _toggleStyle(new ToggleSwitchStyle(this))
    {
        setCheckable(true);
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        setMouseTracking(true);
        setStyle(_toggleStyle);

        QFont textFont = font();
        textFont.setPixelSize(DefaultFontPixelSize);
        setFont(textFont);
    }

    void ToggleSwitch::setOnText(const QString& text)
    {
        _onText = text;
        updateGeometry();
        update();
    }

    void ToggleSwitch::setOffText(const QString& text)
    {
        _offText = text;
        updateGeometry();
        update();
    }

    QSize ToggleSwitch::sizeHint() const
    {
        // 为两种文字共同预留宽度
        const int content = contentWidth();
        const int width = content + HorizontalPadding;
        return {
            qMax(content > TrackWidth ? PreferredTextWidth : MinimumWidth, width),
            qMax(TrackHeight, fontMetrics().height()) + VerticalPadding
        };
    }

    QSize ToggleSwitch::minimumSizeHint() const
    {
        return {MinimumWidth, sizeHint().height()};
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void ToggleSwitch::enterEvent(QEnterEvent* event)
#else
    void ToggleSwitch::enterEvent(QEvent* event)
#endif
    {
        QAbstractButton::enterEvent(event);
        // 控件的留白不属于可点击区域，进入时仍需进行命中判断
        // static_cast<QEnterEvent*> 类型转换用于兼容 Qt5，不可移除，Qt6 类型相同会自动忽略
        // localPos 用于兼容 Qt5 ，在 Qt6 中会自动调用 position，警告可忽略
        _hovered = isEnabled() && hitButton(static_cast<QEnterEvent*>(event)->localPos().toPoint());
        update();
    }

    void ToggleSwitch::leaveEvent(QEvent* event)
    {
        QAbstractButton::leaveEvent(event);
        // 离开只清除悬浮；按压和拖动继续到松手或交互被取消
        _hovered = false;
        update();
    }

    void ToggleSwitch::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event)
        QStyleOptionButton option;
        option.initFrom(this);
        option.state = QStyle::State_None;
        option.state.setFlag(QStyle::State_Enabled, isEnabled());
        option.state.setFlag(QStyle::State_On, isChecked());
        option.state.setFlag(QStyle::State_Off, !isChecked());
        const bool pressed = isDown();
        option.state.setFlag(QStyle::State_MouseOver, _hovered || pressed);
        option.state.setFlag(QStyle::State_Sunken, pressed);

        QPainter painter(this);
        style()->drawControl(QStyle::CE_CheckBox, &option, &painter, this);
    }

    void ToggleSwitch::mousePressEvent(QMouseEvent* event)
    {
        if (isEnabled() && event->button() == Qt::LeftButton && hitButton(event->pos()))
        {
            _pressOffset = _toggleStyle->indicatorOffset();
            _dragOffset = _pressOffset;
            completeInteraction(Completion::Cancel);
            _pressPosition = event->pos();
            _mouseState = MouseState::Pressed;
        }
        QAbstractButton::mousePressEvent(event);
        update();
    }

    void ToggleSwitch::mouseDoubleClickEvent(QMouseEvent* event)
    {
        event->accept();
    }

    void ToggleSwitch::mouseMoveEvent(QMouseEvent* event)
    {
        _hovered = hitButton(event->pos());
        if (_mouseState != MouseState::Idle && (event->buttons() & Qt::LeftButton))
        {
            const int delta = event->pos().x() - _pressPosition.x();
            if (_mouseState != MouseState::Dragging && qAbs(delta) >= DragThreshold)
                _mouseState = MouseState::Dragging;
            if (_mouseState == MouseState::Dragging)
            {
                updateDragPosition(event->pos());
                setDown(true);
                update();
                event->accept();
                return;
            }
        }
        QAbstractButton::mouseMoveEvent(event);
        update();
    }

    void ToggleSwitch::mouseReleaseEvent(QMouseEvent* event)
    {
        if (event->button() != Qt::LeftButton)
        {
            QAbstractButton::mouseReleaseEvent(event);
            return;
        }
        _hovered = hitButton(event->pos());
        if (_mouseState == MouseState::Dragging)
        {
            updateDragPosition(event->pos());
            constexpr qreal midpoint = KnobTravel / 2.0;
            const bool checked = isChecked() ? _dragOffset > midpoint : _dragOffset >= midpoint;
            _mouseState = MouseState::Idle;
            setDown(false);
            setChecked(checked);
            completeInteraction(Completion::DragRelease);
            event->accept();
            emit released();
            emit clicked(isChecked());
            return;
        }
        const bool clicked = isDown() && hitButton(event->pos());
        _mouseState = MouseState::Idle;
        QAbstractButton::mouseReleaseEvent(event);
        completeInteraction(clicked ? Completion::Click : Completion::Cancel);
    }

    bool ToggleSwitch::hitButton(const QPoint& pos) const
    {
        return rect().contains(pos) && rowRect().contains(pos);
    }

    void ToggleSwitch::checkStateSet()
    {
        QAbstractButton::checkStateSet();
        // 包括信号被阻塞的程序状态回退
        _mouseState = MouseState::Idle;
        setDown(false);
        completeInteraction(Completion::Cancel);
    }

    void ToggleSwitch::nextCheckState()
    {
        QAbstractButton::nextCheckState();
        completeInteraction(Completion::Click);
    }

    void ToggleSwitch::hideEvent(QHideEvent* event)
    {
        setDown(false);
        QAbstractButton::hideEvent(event);
    }

    QString ToggleSwitch::displayText() const
    {
        return isChecked() ? _onText : _offText;
    }

    int ToggleSwitch::contentWidth() const
    {
        // 分数缩放下向上取整，避免恰好容纳文本的列被整数测量舍入后误判为需要省略
        const QFontMetricsF metrics(fontMetrics());
        const int textWidth = qCeil(qMax(metrics.horizontalAdvance(_onText), metrics.horizontalAdvance(_offText)));
        return TrackWidth + (textWidth > 0 ? TextSpacing + textWidth : 0);
    }

    QRect ToggleSwitch::rowRect() const
    {
        return {
            HorizontalMargin,
            0,
            qMin(contentWidth(), qMax(0, width() - HorizontalPadding)),
            qMax(TrackHeight, fontMetrics().height()) + VerticalPadding
        };
    }

    QRectF ToggleSwitch::trackRect() const
    {
        const QRect row = rowRect();
        return {qreal(row.left()), row.y() + (row.height() - TrackHeight) / 2.0, TrackWidth, TrackHeight};
    }

    QRect ToggleSwitch::textRect() const
    {
        QRect row = rowRect();
        row.setLeft(row.left() + TextOffset);
        return row;
    }

    void ToggleSwitch::completeInteraction(Completion kind)
    {
        _completion = kind;
        ++_completionSerial;
        update();
    }

    void ToggleSwitch::updateDragPosition(const QPoint& position)
    {
        const qreal delta = position.x() - _pressPosition.x();
        _dragOffset = qBound(0.0, _pressOffset + delta, KnobTravel);
        update();
    }
} // fancy
