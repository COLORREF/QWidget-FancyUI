#include "SelectorBar.h"

#include <QDebug>
#include <QButtonGroup>
#include <QVBoxLayout>

#include "SelectorBarItem.h"
#include "private/SelectorBarScrollArea.h"
#include "private/SelectorBarIndicatorLayer.h"

namespace fancy
{
    SelectorBar::SelectorBar(QWidget *parent) :
        QWidget(parent),
        _scrollArea(new detail::SelectorBarScrollArea(this)),
        _indicatorLayer(new detail::SelectorBarIndicatorLayer(_scrollArea->widget())),
        _buttonGroup(new QButtonGroup(this))
    {
        _buttonGroup->setExclusive(true);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        auto *outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(0, 0, 0, 0);
        outerLayout->addWidget(_scrollArea);

        connect(_buttonGroup, &QButtonGroup::buttonToggled, this, &SelectorBar::syncSelection);
        connect(_scrollArea, &detail::SelectorBarScrollArea::sizeHintChanged, this, &QWidget::updateGeometry);
    }

    SelectorBar::~SelectorBar()
    {
        // 先销毁覆盖层，停止动画和项目事件监听。
        delete _indicatorLayer;
        for (const auto *item: _items)
            disconnect(item, nullptr, this, nullptr);
        disconnect(_buttonGroup, nullptr, this, nullptr);
        disconnect(_scrollArea, nullptr, this, nullptr);
    }

    void SelectorBar::addItem(SelectorBarItem *item)
    {
        insertItem(_items.size(), item);
    }

    SelectorBarItem *SelectorBar::addItem(const QString &text)
    {
        auto *item = new SelectorBarItem(text);
        addItem(item);
        return item;
    }

    SelectorBarItem *SelectorBar::addItem(const QString &text, const IconId &icon)
    {
        auto *item = new SelectorBarItem(text);
        item->setIcon(icon);
        addItem(item);
        return item;
    }

    SelectorBarItem *SelectorBar::addItem(const QString &text, const QIcon &icon)
    {
        auto *item = new SelectorBarItem(text);
        item->setIcon(icon);
        addItem(item);
        return item;
    }

    void SelectorBar::insertItem(int index, SelectorBarItem *item)
    {
        if (!item || _items.contains(item))
            return;

        // 转移按钮组归属，旧栏的布局和项目列表同步移除
        if (auto *group = item->group())
        {
            if (auto *owner = qobject_cast<SelectorBar *>(group->parent()))
                owner->removeItem(item);
            else
                group->removeButton(item);
        }

        index = qBound(0, index, _items.size());
        _scrollArea->insertWidget(index, item);
        item->setChecked(false);
        _items.insert(index, item);
        _buttonGroup->addButton(item);

        connect(item, &QObject::destroyed, this, [this, item] {
            const int index = _items.indexOf(item);
            _items.removeOne(item);
            // QAbstractButton 析构时已从按钮组移除
            if (!selectedItem() && !_items.isEmpty())
                setSelectedItem(_items.at(qBound(0, index, _items.size() - 1)));
            syncSelection();
        });
        connect(item, &QAbstractButton::clicked, this, [this, item] {
            // 手动滚动后再次点击已选项目，也应能看到选中指示条
            if (selectedItem() == item)
                _scrollArea->ensureItemVisible(item);
        });
        syncSelection();
    }

    void SelectorBar::removeItem(SelectorBarItem *item, bool selectFirst)
    {
        if (!_items.contains(item))
            return;
        const int index = _items.indexOf(item);
        _buttonGroup->removeButton(item);
        _items.removeOne(item);
        disconnect(item, nullptr, this, nullptr);
        _scrollArea->removeWidget(item);
        item->setChecked(false);
        // 按参数选择第一项或相邻项；保留回调已指定的选择。
        if (!selectedItem() && !_items.isEmpty())
            setSelectedItem(_items.at(selectFirst ? 0 : qMin(index, _items.size() - 1)));
        syncSelection();
    }

    int SelectorBar::itemCount() const
    {
        return _items.size();
    }

    SelectorBarItem *SelectorBar::itemAt(int index) const
    {
        return _items.value(index, nullptr);
    }

    SelectorBarItem *SelectorBar::selectedItem() const
    {
        return qobject_cast<SelectorBarItem *>(_buttonGroup->checkedButton());
    }

    void SelectorBar::setSelectedItem(SelectorBarItem *item)
    {
        if (!item)
            return; // 非空栏不允许清空；空栏本身已无选中项
        if (item && !_items.contains(item))
        {
            qWarning() << "SelectorBar::setSelectedItem: item is not in this bar";
            return;
        }
        if (selectedItem() == item)
            return;

        item->setChecked(true);
        syncSelection();
    }

    void SelectorBar::syncSelection()
    {
        // 回调可能再次改选；以组的当前状态为准，而非 buttonToggled 的参数
        auto *item = selectedItem();
        if (!item && !_items.isEmpty())
        {
            _items.first()->setChecked(true);
            return; // buttonToggled 会同步选择并发出通知，避免重复通知
        }
        if (_lastNotifiedItem == item)
            return;
        _lastNotifiedItem = item;
        if (item)
            _scrollArea->ensureItemVisible(item);
        _indicatorLayer->select(item);
        emit selectionChanged(item);
    }

    void SelectorBar::setIndicatorSlideEnabled(bool enabled)
    {
        _indicatorLayer->setSlideEnabled(enabled);
    }

    bool SelectorBar::isIndicatorSlideEnabled() const
    {
        return _indicatorLayer->isSlideEnabled();
    }

    void SelectorBar::setHorizontalScrollBarVisible(bool visible)
    {
        _scrollArea->setHorizontalScrollBarVisible(visible);
    }

    bool SelectorBar::isHorizontalScrollBarVisible() const
    {
        return _scrollArea->isHorizontalScrollBarVisible();
    }

    QSize SelectorBar::sizeHint() const
    {
        return _scrollArea->sizeHint();
    }

    QSize SelectorBar::minimumSizeHint() const
    {
        return {0, sizeHint().height()};
    }
} // fancy 命名空间
