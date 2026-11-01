#ifndef QWIDGET_FANCYUI_TOGGLESWITCH_H
#define QWIDGET_FANCYUI_TOGGLESWITCH_H

#include <QAbstractButton>
#include <QRectF>
#include <QStyle>


namespace fancy
{
    class ToggleSwitchStyle;
    class ToggleSwitchIndicator;

    class ToggleSwitch : public QAbstractButton
    {
        Q_OBJECT

        friend class ToggleSwitchStyle;
        friend class ToggleSwitchIndicator;

    public:
        explicit ToggleSwitch(QWidget* parent = nullptr);

        void setOnText(const QString& text);

        void setOffText(const QString& text);

        [[nodiscard]] QString onText() const { return _onText; }
        [[nodiscard]] QString offText() const { return _offText; }
        [[nodiscard]] QSize sizeHint() const override;
        [[nodiscard]] QSize minimumSizeHint() const override;

    protected:
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        void enterEvent(QEnterEvent* event) override;
#else
        void enterEvent(QEvent* event) override;
#endif
        void leaveEvent(QEvent* event) override;
        void paintEvent(QPaintEvent* event) override;
        void mousePressEvent(QMouseEvent* event) override;
        void mouseDoubleClickEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        bool hitButton(const QPoint& pos) const override;
        void checkStateSet() override;
        void nextCheckState() override;
        void hideEvent(QHideEvent* event) override;

    private:
        using QAbstractButton::setText;
        using QAbstractButton::text;

        enum class MouseState
        {
            Idle, Pressed, Dragging
        };

        [[nodiscard]] QString displayText() const;
        [[nodiscard]] int contentWidth() const;
        [[nodiscard]] QRect rowRect() const;
        [[nodiscard]] QRectF trackRect() const;
        [[nodiscard]] QRect textRect() const;

        enum class Completion
        {
            Cancel, Click, DragRelease
        };

        void completeInteraction(Completion kind);
        void updateDragPosition(const QPoint& position);

        QString _onText; // 开启时显示的状态文字，默认无内容，由创建者设置
        QString _offText; // 关闭时显示的状态文字，默认无内容，由创建者设置
        bool _hovered = false; // 鼠标是否位于可点击区域
        MouseState _mouseState = MouseState::Idle; // 当前鼠标按压或拖动阶段
        QPoint _pressPosition; // 按下时的鼠标位置，作为拖动起点
        qreal _pressOffset = 0; // 按下时的指示器位移，单位为逻辑像素
        qreal _dragOffset = 0; // 拖拽输入位置，不存储动画中间值
        quint64 _completionSerial = 0; // 保留交互完成记录，避免重绘合并丢失释放
        Completion _completion = Completion::Cancel;
        ToggleSwitchStyle* _toggleStyle = nullptr;
    };
} // fancy

#endif // QWIDGET_FANCYUI_TOGGLESWITCH_H
