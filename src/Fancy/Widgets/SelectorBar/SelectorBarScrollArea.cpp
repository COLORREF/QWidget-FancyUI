#include "SelectorBarScrollArea_p.h"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QResizeEvent>
#include <QScrollBar>
#include <QWheelEvent>

namespace fancy::detail
{
    SelectorBarScrollArea::SelectorBarScrollArea(QWidget *parent) :
        ScrollArea(parent),
        _contents(new QWidget),
        _layout(new QHBoxLayout(_contents))
    {
        _layout->setContentsMargins(0, 4, 0, 4);
        _layout->setSpacing(0);
        _layout->setAlignment(Qt::AlignLeft);
        _layout->setSizeConstraint(QLayout::SetFixedSize); // 内容保持自然尺寸，空间不足时滚动，不压缩项目
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setWidgetResizable(false);
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setWidget(_contents); // setWidget() 会安装 QScrollArea 的事件过滤器，由本类重载继续处理内容变化
    }

    void SelectorBarScrollArea::insertWidget(int index, QWidget *widget)
    {
        widget->setParent(_contents);
        _layout->insertWidget(index, widget);
        invalidateSizeHint();
    }

    void SelectorBarScrollArea::removeWidget(QWidget *widget)
    {
        _layout->removeWidget(widget);
        widget->setParent(nullptr);
        invalidateSizeHint();
    }

    void SelectorBarScrollArea::ensureItemVisible(const QWidget *item) const
    {
        _layout->activate();
        const auto *view = viewport();
        auto *scrollBar = horizontalScrollBar();
        const int viewportWidth = view->width();
        const int left = item->mapTo(view, QPoint(0, 0)).x();
        const int right = left + item->width();
        int delta = 0;
        if (item->width() > viewportWidth)
            delta = qRound(left + (item->width() - viewportWidth) / 2.0); // 超宽项目居中显示选中指示条
        else if (left < 0)
            delta = left; // 只补足左侧被遮挡的部分
        else if (right > viewportWidth)
            delta = right - viewportWidth; // 完整可见时不滚动
        if (delta != 0 && viewportWidth > 0)
            scrollBar->setValue(scrollBar->value() + (isRightToLeft() ? -delta : delta));
    }

    void SelectorBarScrollArea::setHorizontalScrollBarVisible(bool visible)
    {
        if (isHorizontalScrollBarVisible() == visible)
            return;
        setHorizontalScrollBarPolicy(visible ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
        invalidateSizeHint();
    }

    bool SelectorBarScrollArea::isHorizontalScrollBarVisible() const
    {
        return horizontalScrollBarPolicy() == Qt::ScrollBarAsNeeded;
    }

    QSize SelectorBarScrollArea::sizeHint() const
    {
        QSize size = _layout->sizeHint();
        // 布局包含项目及上下边距；框架也需要预留空间，避免产生额外的纵向滚动范围
        const int frameExtent = 2 * frameWidth();
        if (isHorizontalScrollBarVisible() && size.width() > qMax(0, width() - frameExtent))
            size.rheight() += horizontalScrollBar()->sizeHint().height();
        return size + QSize(frameExtent, frameExtent);
    }

    QSize SelectorBarScrollArea::minimumSizeHint() const
    {
        return {0, sizeHint().height()};
    }

    void SelectorBarScrollArea::invalidateSizeHint()
    {
        const QSize hint = sizeHint();
        if (hint == _lastSizeHint)
            return;
        // 先记录再通知，避免布局调整引发的后续 Resize 对相同尺寸重复通知
        _lastSizeHint = hint;
        updateGeometry();
        emit sizeHintChanged();
    }

    void SelectorBarScrollArea::resizeEvent(QResizeEvent *event)
    {
        ScrollArea::resizeEvent(event);
        invalidateSizeHint();
    }

    bool SelectorBarScrollArea::eventFilter(QObject *watched, QEvent *event)
    {
        // 保留 QScrollArea 更新滚动范围等默认行为，再通知外层更新尺寸提示
        const bool filtered = ScrollArea::eventFilter(watched, event);
        if (watched == _contents && (event->type() == QEvent::LayoutRequest || event->type() == QEvent::Resize))
            invalidateSizeHint();
        return filtered;
    }

    void SelectorBarScrollArea::wheelEvent(QWheelEvent *event)
    {
        QCoreApplication::sendEvent(horizontalScrollBar(), event);
    }
}
