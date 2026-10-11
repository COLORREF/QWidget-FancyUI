//
// A1840 于 2026/10/9 创建。
//

#include "SelectorBarItemStyle.h"

#include <QPainter>
#include <QImage>
#include <QStyleOptionButton>
#include <QVariantAnimation>
#include <QEasingCurve>

#include "Core/ControlState/ControlState.h"
#include "Core/Palette/Palette.h"
#include "utils/General.h"

namespace fancy
{
    namespace
    {
        constexpr int HorizontalPadding = 12; // 内容两侧的留白
        constexpr int TopPadding = 10; // 内容上方的留白
        constexpr int BottomPadding = 7; // 内容与指示条之间的留白
        constexpr int IconTextSpacing = 8; // 图标与文字的间距
        constexpr int DefaultIconSize = 17; // 默认图标可见笔画的最大宽高
        constexpr int DefaultIconLayoutSize = 20; // 默认图标参与布局的宽高
        constexpr int IconHorizontalMargin = -2; // 图标左右两侧的布局边距
        constexpr int IconMeasureSize = 128; // 测量图标透明留白时使用的画布大小
        constexpr int IndicatorWidth = 16; // 选中指示条的宽度
        constexpr int IndicatorInitialWidth = 4; // 指示条入场时的初始宽度
        constexpr int IndicatorHeight = 3; // 选中指示条的高度
        constexpr int IndicatorAnimationDuration = 167; // 指示条入场动画时长，单位为毫秒

        constexpr qreal IconLayoutScale = static_cast<qreal>(DefaultIconLayoutSize) / DefaultIconSize; // 图标布局尺寸与可见尺寸的比例
    }

    SelectorBarItemStyle::SelectorBarItemStyle(SelectorBarItem *parent) :
        QProxyStyle(nullptr),
        _parent(parent),
        _indicatorAnimation(new QVariantAnimation(this))
    {
        setParent(parent);
        parent->setIconSize(QSize(DefaultIconSize, DefaultIconSize));
        _indicatorProgress = parent->isChecked() ? 1.0 : 0.0;
        _indicatorAnimation->setDuration(IndicatorAnimationDuration);
        _indicatorAnimation->setStartValue(0.0);
        _indicatorAnimation->setEndValue(1.0);
        QEasingCurve easing(QEasingCurve::BezierSpline);
        easing.addCubicBezierSegment(QPointF(0, 0), QPointF(0, 1), QPointF(1, 1));
        _indicatorAnimation->setEasingCurve(easing);

        connect(_indicatorAnimation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
            _indicatorProgress = value.toReal();
            _parent->update();
        });
        connect(parent, &QAbstractButton::toggled, this, [this](bool checked) {
            _indicatorAnimation->stop();
            _indicatorProgress = 0.0;
            if (checked)
            {
                if (_parent->isVisible())
                    _indicatorAnimation->start();
                else
                    _indicatorProgress = 1.0; // 显示前设置的初始选中项直接呈现最终状态
            }
            // 取消选中立即隐藏，不反向播放；悬停和重复按下不重启动画
            _parent->update();
        });
    }

    void SelectorBarItemStyle::drawItemText(
        QPainter *painter,
        const QRect &rect,
        int flags,
        const QPalette &pal,
        bool enabled,
        const QString &text,
        QPalette::ColorRole textRole) const
    {
        Q_UNUSED(enabled)
        Q_UNUSED(flags)
        const QFontMetrics metrics(painter->font());
        const auto lines = textLines(text);
        painter->save();
        painter->setPen(pal.color(textRole));
        painter->setClipRect(rect, Qt::IntersectClip);
        const int height = text.isEmpty() ? QSize().height() : metrics.height() + (static_cast<int>(lines.size()) - 1) * metrics.lineSpacing();
        int baseline = rect.y() + (rect.height() - height) / 2 + metrics.ascent();
        for (const auto &line: lines)
        {
            const QString visibleLine = _parent->isTextElideEnabled()
                                            ? metrics.elidedText(line, Qt::ElideRight, qMax(0, rect.width()))
                                            : line;
            painter->drawText(QPoint(rect.x(), baseline), visibleLine);
            baseline += metrics.lineSpacing();
        }
        painter->restore();
    }

    void SelectorBarItemStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        const auto *button = qstyleoption_cast<const QStyleOptionButton *>(option);
        if (!button)
        {
            QProxyStyle::drawControl(element, option, painter, widget);
            return;
        }

        const ControlState state(button->state, ControlType::WidgetCheckable);
        const bool selected = state._on;

        switch (element)
        {
            case CE_PushButton:
            {
                // 绘制状态背景、选中指示条及图标文字
                drawControl(CE_PushButtonBevel, button, painter, widget);
                drawControl(CE_PushButtonLabel, button, painter, widget);
                return;
            }
            case CE_PushButtonBevel:
            {
                painter->fillRect(button->rect, Palette::palette()[ColorRole::SelectorBarItemBackground]);
                if (selected)
                {
                    painter->save();
                    painter->setRenderHint(QPainter::Antialiasing);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(Palette::palette()[state._enable ? ColorRole::AppAccent : ColorRole::DisEnabledText]);
                    painter->setOpacity(painter->opacity() * _indicatorProgress);
                    const qreal indicatorWidth = IndicatorInitialWidth + (IndicatorWidth - IndicatorInitialWidth) * _indicatorProgress;
                    const qreal x = button->rect.x() + (button->rect.width() - indicatorWidth) / 2.0;
                    const int y = button->rect.bottom() - IndicatorHeight + 1;
                    painter->drawRoundedRect(QRectF(x, y, indicatorWidth, IndicatorHeight), 1.5, 1.5);
                    painter->restore();
                }
                return;
            }
            case CE_PushButtonLabel:
            {
                // 图标与文字共用内容区域，分别保留上下内边距和指示条空间
                painter->save();
                painter->setFont(_parent->font());
                const bool hasIcon = !button->icon.isNull();
                const bool hasText = !button->text.isEmpty();
                const QFontMetrics metrics(painter->font());
                const QSize iconSize = iconLayoutSize(button->iconSize, IconLayoutScale);
                const int iconWidth = hasIcon ? qMax(0, iconSize.width() + 2 * IconHorizontalMargin) : 0;
                const int spacing = hasIcon && hasText ? IconTextSpacing : 0;
                const QString &text = button->text;
                const int availableTextWidth = qMax(0, button->rect.width() - 2 * HorizontalPadding - iconWidth - spacing);
                const int textWidth = qMin(textSize(metrics, text).width(), availableTextWidth);
                const int groupWidth = iconWidth + spacing + textWidth;
                const int x = button->rect.x() + (button->rect.width() - groupWidth) / 2;
                const int contentTop = button->rect.y() + TopPadding;
                const int contentHeight = qMax(0, button->rect.height() - TopPadding - BottomPadding - IndicatorHeight);

                if (hasIcon)
                {
                    const QRectF iconRect(x + IconHorizontalMargin, contentTop + (contentHeight - iconSize.height()) / 2.0,
                                          iconSize.width(), iconSize.height());
                    QIcon::Mode mode = QIcon::Normal;
                    switch (state)
                    {
                        case VisualState::Disabled:
                        case VisualState::SelectedDisabled:
                            mode = QIcon::Disabled;
                            break;
                        case VisualState::Hover:
                        case VisualState::SelectedHover:
                            mode = QIcon::Active;
                            break;
                        case VisualState::Pressed:
                        case VisualState::SelectedPressed:
                            mode = QIcon::Selected;
                            break;
                        default:
                            break;
                    }
                    // 补偿 SVG 自带的透明留白，保持宽高比，使可见笔画符合 iconSize
                    if (const QRectF ink = iconInkBounds(button->icon); !ink.isEmpty())
                    {
                        const qreal scale = qMin(button->iconSize.width() / ink.width(), button->iconSize.height() / ink.height());
                        painter->save();
                        painter->translate(iconRect.center());
                        painter->scale(scale, scale);
                        painter->translate(-ink.center());
                        button->icon.paint(painter, {0, 0, IconMeasureSize, IconMeasureSize},
                                           Qt::AlignCenter, mode, selected ? QIcon::On : QIcon::Off);
                        painter->restore();
                    }
                }
                if (hasText)
                {
                    auto textRole = ColorRole::Text;
                    switch (state)
                    {
                        case VisualState::Normal:
                            textRole = ColorRole::Text;
                            break;
                        case VisualState::Hover:
                            textRole = ColorRole::SelectorBarItemTextHover;
                            break;
                        case VisualState::Pressed:
                            textRole = ColorRole::SelectorBarItemTextPressed;
                            break;
                        case VisualState::Disabled:
                        case VisualState::SelectedDisabled:
                            textRole = ColorRole::DisEnabledText;
                            break;
                        case VisualState::Selected:
                            textRole = ColorRole::SelectorBarItemTextSelected;
                            break;
                        case VisualState::SelectedHover:
                            textRole = ColorRole::SelectorBarItemTextSelectedHover;
                            break;
                        case VisualState::SelectedPressed:
                            textRole = ColorRole::SelectorBarItemTextSelectedPressed;
                            break;
                        default:
                            break;
                    }
                    QPalette textPalette = button->palette;
                    textPalette.setColor(QPalette::ButtonText, Palette::palette()[textRole]);
                    const QRect textRect(x + iconWidth + spacing, contentTop, textWidth, contentHeight);
                    drawItemText(painter, textRect, Qt::AlignLeft | Qt::AlignVCenter, textPalette, state._enable, text, QPalette::ButtonText);
                }
                painter->restore();
                return;
            }
            default:
                QProxyStyle::drawControl(element, option, painter, widget);
        }
    }

    QSize SelectorBarItemStyle::sizeHint() const
    {
        const QFontMetrics metrics(_parent->font());
        const bool hasIcon = !_parent->icon().isNull();
        const bool hasText = !_parent->text().isEmpty();
        const QSize labelSize = textSize(metrics, _parent->text());
        const int contentWidth = labelSize.width()
                                 + (hasIcon ? qMax(0, iconLayoutSize(_parent->iconSize(), IconLayoutScale).width() + 2 * IconHorizontalMargin) : 0)
                                 + (hasIcon && hasText ? IconTextSpacing : 0);
        const int contentHeight = qMax(labelSize.height(), hasIcon ? iconLayoutSize(_parent->iconSize(), IconLayoutScale).height() : 0);
        return {contentWidth + 2 * HorizontalPadding, contentHeight + TopPadding + BottomPadding + IndicatorHeight};
    }

    QRectF SelectorBarItemStyle::iconInkBounds(const QIcon &icon) const
    {
        if (_measuredIconKey == icon.cacheKey() && !_iconInkBounds.isEmpty())
            return _iconInkBounds;

        // 仅在图标变化时测量，交互状态切换时复用边界，避免图标跳动
        QImage image(IconMeasureSize, IconMeasureSize, QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        QPainter painter(&image);
        icon.paint(&painter, image.rect(), Qt::AlignCenter, QIcon::Normal, QIcon::Off);
        painter.end();
        QRect bounds;
        for (int y = 0; y < image.height(); ++y)
        {
            const auto *pixels = reinterpret_cast<const QRgb *>(image.constScanLine(y));
            for (int x = 0; x < image.width(); ++x)
                if (qAlpha(pixels[x]) != 0)
                    bounds |= QRect(x, y, 1, 1);
        }
        _measuredIconKey = icon.cacheKey();
        _iconInkBounds = bounds.isEmpty() ? QRectF(image.rect()) : QRectF(bounds);
        return _iconInkBounds;
    }
} // fancy 命名空间
