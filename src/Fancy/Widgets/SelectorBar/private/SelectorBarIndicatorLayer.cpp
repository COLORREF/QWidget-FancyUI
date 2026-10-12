#include "SelectorBarIndicatorLayer.h"

#include <QEvent>
#include <QPainter>
#include "../SelectorBarItem.h"
#include "Core/Palette/Palette.h"

namespace fancy::detail
{
    namespace
    {
        constexpr int RevealDuration = 167; // 入场动画时长，单位毫秒
        constexpr int SlideDuration = 600; // 跨项滑动时长，单位毫秒
        constexpr qreal IndicatorThickness = 3.0; //指示条厚度，逻辑像素
    }

    SelectorBarIndicatorLayer::SelectorBarIndicatorLayer(QWidget *contents) :
        QWidget(contents),
        _animation(this)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setAttribute(Qt::WA_NoSystemBackground);
        setFocusPolicy(Qt::NoFocus);
        setGeometry(contents->rect());
        contents->installEventFilter(this);
        hide();
        connect(&_animation, &QVariantAnimation::valueChanged, this, QOverload<>::of(&SelectorBarIndicatorLayer::update));
        connect(&_animation, &QVariantAnimation::finished, this, &SelectorBarIndicatorLayer::finish);
        connect(&Palette::palette(), &Palette::appThemeChange, this, QOverload<>::of(&SelectorBarIndicatorLayer::update));
        connect(&Palette::palette(), &Palette::appColorChange, this, QOverload<>::of(&SelectorBarIndicatorLayer::update));
    }

    QLineF SelectorBarIndicatorLayer::indicatorLine(const QRectF &itemRect)
    {
        const qreal centerY = itemRect.bottom() - IndicatorThickness / 2;
        return {itemRect.center().x() - 8, centerY, itemRect.center().x() + 8, centerY};
    }

    QLineF SelectorBarIndicatorLayer::itemLine(const SelectorBarItem *item) const
    {
        return indicatorLine(item == parentWidget() ? QRectF(item->rect()) : QRectF(item->geometry()));
    }

    QLineF SelectorBarIndicatorLayer::currentLine() const
    {
        if (!_selected)
            return {};
        return _animation.geometry(itemLine(_selected));
    }

    void SelectorBarIndicatorLayer::refresh()
    {
        setGeometry(parentWidget()->rect());
        setVisible(_selected && _selected->isChecked() && !_selected->isHidden());
        raise();
        update();
    }

    void SelectorBarIndicatorLayer::finish()
    {
        _animation.reveal(false, 4.0, RevealDuration);
        refresh();
    }

    void SelectorBarIndicatorLayer::setSlideEnabled(bool enabled)
    {
        if (_enabled == enabled)
            return;
        _enabled = enabled;
        if (_animation.isSliding())
            finish();
    }

    void SelectorBarIndicatorLayer::select(SelectorBarItem *item)
    {
        if (_selected == item)
        {
            if (!item)
                finish();
            return;
        }
        const bool canSlide = _enabled && _selected && item && _selected->isVisible() && item->isVisible()
                              && _selected->parentWidget() == parentWidget() && item->parentWidget() == parentWidget();
        /** @note 从旧选中项的完整线段重新开始，避免反向切换继承拉长的中间线段。 */
        const QLineF from = canSlide ? itemLine(_selected) : QLineF();
        _animation.stop();
        if (_selected && _selected != parentWidget())
            _selected->removeEventFilter(this);
        _selected = item;
        if (item && item != parentWidget())
            item->installEventFilter(this);
        if (canSlide)
            _animation.slide(from, itemLine(item), Qt::Horizontal, SlideDuration);
        else
            _animation.reveal(item && item->isVisible(), 4.0, RevealDuration);
        refresh();
    }

    bool SelectorBarIndicatorLayer::eventFilter(QObject *watched, QEvent *event)
    {
        if (watched == _selected && event->type() == QEvent::Destroy)
        {
            _selected = nullptr;
            finish();
            return false;
        }
        if (watched == _selected && event->type() == QEvent::ParentChange &&
            _selected != parentWidget() && _selected->parentWidget() != parentWidget())
        {
            select(nullptr);
            return false;
        }
        switch (event->type())
        {
            case QEvent::Hide:
                finish();
                break;
            case QEvent::Show:
            case QEvent::Move:
            case QEvent::Resize:
            case QEvent::LayoutRequest:
                /** @note 滚动只移动 contents；目标在内容坐标中重排时才停止滑动。 */
                if (_selected && _animation.isSliding() && itemLine(_selected) != _animation.target())
                    finish();
                else
                    refresh();
                break;
            case QEvent::EnabledChange:
                update();
                break;
            default:
                break;
        }
        return QWidget::eventFilter(watched, event);
    }

    void SelectorBarIndicatorLayer::paintEvent(QPaintEvent *)
    {
        if (!_selected || !_selected->isChecked())
            return;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QColor color = Palette::palette()[_selected->isEnabled() ? ColorRole::AppAccent : ColorRole::DisEnabledText];
        painter.setPen(QPen(color, IndicatorThickness, Qt::SolidLine, Qt::RoundCap));
        painter.setOpacity(_animation.opacity());
        // 动画线段的两端表示最终可见边界，而非画笔中心线端点
        // RoundCap 在端点外侧和垂直方向各延伸半个线宽，因此左右端点各内缩 1.5：
        // 可见长度 16 对应中心线长度 13，入场可见长度 4 对应中心线长度 1
        // Y 始终位于厚度中心，使用非 cosmetic 的 3px 画笔及浮点坐标，DPR 由 Qt 处理
        // 最短可见长度为 4，大于线宽 3；若以后允许更短，应避免内缩后端点倒置
        const QLineF visibleLine = currentLine();
        constexpr qreal halfPen = IndicatorThickness / 2;
        painter.drawLine(QLineF(visibleLine.p1() + QPointF(halfPen, 0), visibleLine.p2() - QPointF(halfPen, 0)));
    }
}
