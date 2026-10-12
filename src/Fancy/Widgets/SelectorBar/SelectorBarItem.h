#ifndef QWIDGET_FANCYUI_SELECTORBARITEM_H
#define QWIDGET_FANCYUI_SELECTORBARITEM_H

#include <QAbstractButton>
#include <optional>

#include "Core/Defs.h"

namespace fancy
{
    namespace detail { class SelectorBarIndicatorLayer; }
    class SelectorBarItem : public QAbstractButton
    {
        Q_OBJECT
        Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
        Q_PROPERTY(bool textElideEnabled READ isTextElideEnabled WRITE setTextElideEnabled)

    public:
        explicit SelectorBarItem(QWidget *parent = nullptr);

        explicit SelectorBarItem(const QString &text, QWidget *parent = nullptr);

        /** @brief 使用调用者提供的图标，不补充状态配色，并停止此前的自动主题适配 */
        void setIcon(const QIcon &icon);

        /** @brief 使用库图标，自动生成各状态配色并随主题更新 */
        void setIcon(const IconId &id);

        void setIcon(AntDesignIcons icon) { setIcon(iconId(icon)); }
        void setIcon(BootstrapIcons icon) { setIcon(iconId(icon)); }
        void setIcon(IconPark icon) { setIcon(iconId(icon)); }

        void setTextElideEnabled(bool enabled); // 宽度不足时逐行右侧省略，默认关闭；配合 QWidget 的最大宽度使用

        [[nodiscard]] bool isTextElideEnabled() const;

        [[nodiscard]] QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        friend class SelectorBar;
        void setIndicatorManagedByBar(bool managed);
        void refreshIconColors();

        detail::SelectorBarIndicatorLayer *_localIndicator = nullptr;
        bool _indicatorManagedByBar = false;
        std::optional<IconId> _iconId;
        bool _textElideEnabled = false;
    };
} // fancy

#endif //QWIDGET_FANCYUI_SELECTORBARITEM_H
