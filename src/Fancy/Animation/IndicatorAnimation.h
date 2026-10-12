#ifndef FANCY_INDICATORANIMATION_H
#define FANCY_INDICATORANIMATION_H

#include <QVariantAnimation>
#include <QLineF>

namespace fancy
{
    /**
     * @brief 提供指示器入场与跨项滑动的几何和透明度动画
     */
    class IndicatorAnimation : public QVariantAnimation
    {
        Q_OBJECT

    public:
        explicit IndicatorAnimation(QObject *parent = nullptr);

        /**
         * @brief 从中心展开并淡入。
         * @param animated 为 false 时立即完成动画
         * @param initialLength 初始可见长度；绘制时限制在 0 到目标长度之间
         * @param msecs 本次动画时长，单位为毫秒，默认 167；非正值表示立即完成
         * @note 目标线段由 geometry() 提供，布局改变无需重启动画
         */
        void reveal(bool animated, qreal initialLength = 4.0, int msecs = 167);

        /**
         * @brief 先拉伸后收拢至目标线段。
         * @param from 起始线段，与 to 使用同一坐标系。
         * @param to 目标线段
         * @param orientation 线段方向，默认为水平
         * @param msecs 本次时长，单位为毫秒，默认 600；非正值表示立即完成
         * @note 端点按水平从左到右、垂直从上到下的顺序统一；支持正反向切换
         *       快速改选时采用旧选中项还是当前动画几何作为 from，由调用者决定
         */
        void slide(const QLineF &from, const QLineF &to, Qt::Orientation orientation = Qt::Horizontal, int msecs = 600);

        [[nodiscard]] bool isSliding() const { return _mode == Mode::Slide && state() == Running; }

        /**
         * @brief 返回最近一次 slide() 设置的目标线段
         */
        [[nodiscard]] QLineF target() const { return _to; }

        /**
         * @brief 返回当前指示器线段的可见边界。
         * @param target 当前布局中的目标线段。入场及静态时使用它；滑动时使用 slide() 保存的端点。
         * @return 当前线段，端点表示可见边界。
         * @note 线宽、圆头补偿及裁剪由调用者处理。可将 valueChanged 连接到绘制层的 update()。
         */
        [[nodiscard]] QLineF geometry(const QLineF &target) const;

        /**
         * @brief 返回当前透明度；滑动时恒为 1
         */
        [[nodiscard]] qreal opacity() const;

    private:
        enum class Mode { Reveal, Slide };

        Mode _mode = Mode::Reveal;
        QLineF _from;
        QLineF _to;
        QEasingCurve _reveal;
        QEasingCurve _stretch;
        QEasingCurve _settle;
        Qt::Orientation _orientation = Qt::Horizontal;
        qreal _initialLength = 4.0;
    };
}
#endif
