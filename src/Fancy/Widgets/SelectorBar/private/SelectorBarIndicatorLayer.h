#ifndef FANCY_SELECTORBARINDICATORLAYER_H
#define FANCY_SELECTORBARINDICATORLAYER_H

#include <QPointer>
#include <QWidget>
#include "Animation/IndicatorAnimation.h"

namespace fancy
{
    class SelectorBarItem;

    namespace detail
    {
        /**
         * @brief 绘制指示器的入场、跨项运动及静态状态
         * @note 不参与布局和鼠标命中，统一使用 SelectorBar 滚动内容坐标。
         */
        class SelectorBarIndicatorLayer : public QWidget
        {
        public:
            explicit SelectorBarIndicatorLayer(QWidget *contents);

            void select(SelectorBarItem *item);

            void setSlideEnabled(bool enabled);

            [[nodiscard]] bool isSlideEnabled() const { return _enabled; }


            bool eventFilter(QObject *watched, QEvent *event) override;

        protected:
            void paintEvent(QPaintEvent *event) override;

        private:
            /**
             * @brief 计算最终指示条的可见边界；动画只接收该水平线段
             */
            static QLineF indicatorLine(const QRectF &itemRect);

            QLineF itemLine(const SelectorBarItem *item) const;

            [[nodiscard]] QLineF currentLine() const;

            void refresh();

            void finish();

            IndicatorAnimation _animation;
            QPointer<SelectorBarItem> _selected;
            bool _enabled = true;
        };
    }
}
#endif
