//
// A1840 于 2026/10/9 创建。
//

#ifndef QWIDGET_FANCYUI_SELECTORBARITEMSTYLE_H
#define QWIDGET_FANCYUI_SELECTORBARITEMSTYLE_H
#include <QProxyStyle>

#include "SelectorBarItem.h"

class QVariantAnimation;

namespace fancy
{
    class SelectorBarItemStyle : public QProxyStyle
    {
        Q_OBJECT

    public:
        explicit SelectorBarItemStyle(SelectorBarItem *parent);

        void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;

        void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        [[nodiscard]] QSize sizeHint() const;

    private:
        QRectF iconInkBounds(const QIcon &icon) const;

        SelectorBarItem *_parent;
        QVariantAnimation *_indicatorAnimation;
        qreal _indicatorProgress = 0.0; // 指示条展开和淡入的共同进度
        mutable qint64 _measuredIconKey = 0; // 已测量图标的缓存标识
        mutable QRectF _iconInkBounds; // 图标笔画在测量画布中的边界
    };
} // fancy 命名空间

#endif // 头文件保护结束
