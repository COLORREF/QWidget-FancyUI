#ifndef QWIDGET_FANCYUI_SELECTORBARSCROLLAREA_H
#define QWIDGET_FANCYUI_SELECTORBARSCROLLAREA_H

#include "Widgets/ScrollArea/ScrollArea.h"

class QHBoxLayout;

namespace fancy::detail
{
    /**
     * @brief SelectorBar 内部使用的横向滚动区域，不作为公共控件使用
     * @note 负责内容容器、布局、尺寸提示及滚动；项目归属校验、按钮组和选择状态由 SelectorBar 管理
     */
    class SelectorBarScrollArea : public ScrollArea
    {
        Q_OBJECT

    public:
        explicit SelectorBarScrollArea(QWidget *parent = nullptr);

        /**
         * @brief 将控件加入内容布局，并将其父对象设置为内容容器
         * @note 调用方应先完成旧栏的移除；此处不操作按钮组、选择状态或业务信号
         */
        void insertWidget(int index, QWidget *widget);

        /**
         * @brief 将控件移出内容布局并清空父对象，不销毁控件
         * @note 调用方负责更新项目归属及选择状态，并决定控件后续生命周期
         */
        void removeWidget(QWidget *widget);

        /**
         * @brief 滚动至指定项目可见；超宽项目居中，完整可见的项目不移动
         */
        void ensureItemVisible(const QWidget *item) const;

        /**
         * @brief 设置是否允许在内容溢出时显示横向滚动条，隐藏时仍可滚动
         */
        void setHorizontalScrollBarVisible(bool visible);

        /**
         * @brief 返回是否允许按需显示横向滚动条，不表示此刻实际可见
         */
        [[nodiscard]] bool isHorizontalScrollBarVisible() const;

        [[nodiscard]] QSize sizeHint() const override;

        [[nodiscard]] QSize minimumSizeHint() const override;

    signals:
        /**
         * @brief 尺寸提示的计算结果变化时，通知外层同步更新
         */
        void sizeHintChanged();

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        /**
         * @brief 将内容区域的滚轮事件交给横向滚动条处理
         * @note Qt 默认按 angleDelta 的方向选择滚动条，隐藏纵向滚动条并不会自动改为横向滚动
         *       此处仅调整事件接收者，滚动步长、修饰键及小幅角度增量的累计仍由 Qt 处理
         */
        void wheelEvent(QWheelEvent *event) override;

    private:
        /**
         * @brief 仅在尺寸提示变化时更新布局协商，避免重复通知内外层布局
         */
        void invalidateSizeHint();

        QWidget *_contents;
        QHBoxLayout *_layout;
        QSize _lastSizeHint; // 上次参与布局通知的尺寸提示，不在 sizeHint() 查询时更新
    };
}

#endif // QWIDGET_FANCYUI_SELECTORBARSCROLLAREA_H
