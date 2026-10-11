#include "SelectorBar.h"

#include <QDebug>
#include <QButtonGroup>
#include <QSignalBlocker>
#include <QVBoxLayout>

#include "SelectorBarItem.h"
#include "SelectorBarScrollArea_p.h"

namespace fancy
{
    SelectorBar::SelectorBar(QWidget *parent) :
        QWidget(parent),
        _scrollArea(new detail::SelectorBarScrollArea(this)),
        _buttonGroup(new QButtonGroup(this))
    {
        _buttonGroup->setExclusive(true);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        auto *outerLayout = new QVBoxLayout(this);
        outerLayout->setContentsMargins(0, 0, 0, 0);
        outerLayout->addWidget(_scrollArea);

        connect(_buttonGroup, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled), this, [this] { syncSelection(); });
        connect(_scrollArea, &detail::SelectorBarScrollArea::sizeHintChanged, this, &QWidget::updateGeometry);
    }

    SelectorBar::~SelectorBar()
    {
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
            _items.removeOne(item);
            // QAbstractButton 析构时已从按钮组移除
            syncSelection();
        });
        connect(item, &QAbstractButton::clicked, this, [this, item] {
            // 手动滚动后再次点击已选项目，也应能看到选中指示条。
            if (selectedItem() == item)
                _scrollArea->ensureItemVisible(item);
        });
    }

    void SelectorBar::removeItem(SelectorBarItem *item)
    {
        if (!_items.contains(item))
            return;
        const bool wasSelected = selectedItem() == item;
        _buttonGroup->removeButton(item);
        _items.removeOne(item);
        disconnect(item, nullptr, this, nullptr);
        _scrollArea->removeWidget(item);
        item->setChecked(false);
        // 选中项被移除后优先选择剩余第一项；回调已指定新选择时保留其结果。
        if (wasSelected && !selectedItem() && !_items.isEmpty())
            setSelectedItem(_items.first());
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
        if (item && !_items.contains(item))
        {
            qWarning() << "SelectorBar::setSelectedItem: item is not in this bar";
            return;
        }
        if (selectedItem() == item)
            return;

        if (item)
            item->setChecked(true);
        else
        {
            // 互斥组不允许直接取消唯一选中项；恢复互斥后再通知栏的使用者
            const QSignalBlocker blocker(_buttonGroup);
            _buttonGroup->setExclusive(false);
            _buttonGroup->checkedButton()->setChecked(false);
            _buttonGroup->setExclusive(true);
        }
        syncSelection();
    }

    void SelectorBar::syncSelection()
    {
        // 回调可能再次改选；以组的当前状态为准，而非 buttonToggled 的参数
        auto *item = selectedItem();
        if (_lastNotifiedItem == item)
            return;
        _lastNotifiedItem = item;
        if (item)
            _scrollArea->ensureItemVisible(item);
        emit selectionChanged(item);
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
