#ifndef QWIDGET_FANCYUI_SELECTORBAR_H
#define QWIDGET_FANCYUI_SELECTORBAR_H

#include <QList>
#include <QWidget>

#include "Core/Defs.h"

class QButtonGroup;

namespace fancy
{
    class SelectorBarItem;

    namespace detail
    {
        class SelectorBarScrollArea;
        class SelectorBarIndicatorLayer;
    }

    class SelectorBar : public QWidget
    {
        Q_OBJECT
        Q_PROPERTY(bool horizontalScrollBarVisible READ isHorizontalScrollBarVisible WRITE setHorizontalScrollBarVisible)
        Q_PROPERTY(bool indicatorSlideEnabled READ isIndicatorSlideEnabled WRITE setIndicatorSlideEnabled)

    public:
        explicit SelectorBar(QWidget *parent = nullptr);

        ~SelectorBar() override;

        /**
         * @brief 在末尾添加项目
         * @param item 要添加的项目；为空或已属于当前栏时不执行任何操作
         * @note 项目属于其他 SelectorBar 时，会自动从旧栏移除，再转移至当前栏
         * - 旧栏会同步清理按钮组、布局、项目列表以及项目到旧栏的信号连接
         * - 若 item 原先在旧栏选中，旧栏自动选中移除后的第一项，并发出 selectionChanged(新选中项)
         * - 若旧栏已无项目，则清空选择并发出 selectionChanged(nullptr)；移除未选中项不改变旧栏选择
         * - 添加后 item 的父对象设为当前栏的内容容器，包括从其他栏转入的 item；其生命周期由当前栏管理，随当前栏销毁
         * - item 加入后处于未选中状态，当前栏原有选择不变；若后续移除或转移，则按新的归属管理生命周期
         * - 项目属于普通 QButtonGroup 时，先退出原按钮组；其外部布局和业务记录由使用者管理
         */
        void addItem(SelectorBarItem *item);

        /**
         * @brief 创建纯文本项目并添加到末尾
         * @param text 项目文本
         * @return 新建的项目，可用于继续设置属性或指定选择
         * @note 生命周期由当前栏管理；添加后未选中，当前栏原有选择不变
         * @see addItem(SelectorBarItem *)
         */
        SelectorBarItem *addItem(const QString &text);

        /**
         * @brief 创建带图标的文本项目并添加到末尾
         * @param text 项目文本
         * @param icon 项目图标，使用 SelectorBarItem 的主题颜色适配逻辑
         * @return 新建的项目，生命周期及选择规则与纯文本重载相同
         */
        SelectorBarItem *addItem(const QString &text, const IconId &icon);

        /**
         * @brief 使用调用者提供的 QIcon 创建项目并添加到末尾
         * @param text 项目文本
         * @param icon 项目图标，各状态的图像及配色由调用者提供，不自动生成或随主题重建
         * @return 新建的项目，生命周期及选择规则与纯文本重载相同
         * @note 未提供的状态由 Qt 按默认规则回退或生成效果
         */
        SelectorBarItem *addItem(const QString &text, const QIcon &icon);

        /** @brief 使用枚举图标创建项目，状态配色和主题适配规则与 IconId 重载相同 */
        SelectorBarItem *addItem(const QString &text, AntDesignIcons icon) { return addItem(text, iconId(icon)); }
        SelectorBarItem *addItem(const QString &text, BootstrapIcons icon) { return addItem(text, iconId(icon)); }
        SelectorBarItem *addItem(const QString &text, IconPark icon) { return addItem(text, iconId(icon)); }

        /**
         * @brief 在指定位置插入项目
         * @param index 插入位置，超出范围时限制在 [0, itemCount()] 内
         * @param item 要插入的项目；为空或已属于当前栏时不执行任何操作，不调整已有 item 顺序
         * @note 项目归属、跨栏转移和选中状态的处理与 addItem() 相同。
         * @see addItem
         */
        void insertItem(int index, SelectorBarItem *item);

        /**
         * @brief 移除 item，但不销毁
         * @param item 要移除的项目；不属于当前栏时不执行任何操作
         * @note 移除后 item 退出按钮组和布局，断开 item 到当前栏的信号连接，父对象设为空并取消选中
         * - 若移除的是选中项，自动选中剩余第一项；无剩余项目时清空选择，发出相应的 selectionChanged
         * - 移除未选中项不改变当前选择；若移除过程中的回调已指定新选择，则保留该选择
         * - 使用者负责后续销毁项目，或将其加入其他容器
         */
        void removeItem(SelectorBarItem *item);

        [[nodiscard]] int itemCount() const;

        [[nodiscard]] SelectorBarItem *itemAt(int index) const;

        [[nodiscard]] SelectorBarItem *selectedItem() const;

        void setSelectedItem(SelectorBarItem *item);

        /**
         * @brief 设置是否允许按需显示横向滚动条
         * @param visible true 在内容宽度超出可用空间时显示，false 始终隐藏；默认 false
         * @note 隐藏滚动条不禁用滚动，仍可使用滚轮和选中项定位
         */
        void setHorizontalScrollBarVisible(bool visible);

        /**
         * @brief 返回是否允许按需显示横向滚动条，不表示滚动条此刻实际可见
         */
        [[nodiscard]] bool isHorizontalScrollBarVisible() const;

        /**
         * @brief 设置跨项拉伸滑动，默认开启；关闭后使用入场动画，首次选中仍使用入场动画。
         *  @note 开关不改变选择，不发出 selectionChanged；中途关闭会立即定位到当前选中项
         */
        void setIndicatorSlideEnabled(bool enabled);

        [[nodiscard]] bool isIndicatorSlideEnabled() const;

        [[nodiscard]] QSize sizeHint() const override;

        [[nodiscard]] QSize minimumSizeHint() const override;

    signals:
        void selectionChanged(SelectorBarItem *item);

    private:
        void syncSelection();

        detail::SelectorBarScrollArea *_scrollArea; // 管理内容布局与滚动，不参与选择决策
        detail::SelectorBarIndicatorLayer *_indicatorLayer;
        QButtonGroup *_buttonGroup;
        QList<SelectorBarItem *> _items;
        SelectorBarItem *_lastNotifiedItem = nullptr; // 仅用于去重通知，选择状态由按钮组维护
    };
} // fancy 命名空间

#endif // QWIDGET_FANCYUI_SELECTORBAR_H
