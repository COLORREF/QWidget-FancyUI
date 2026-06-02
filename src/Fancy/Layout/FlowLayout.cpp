//
// Created by TX on 2025/9/7.
//

#include "FlowLayout.h"

namespace fancy
{
    FlowLayout::FlowLayout(QWidget *parent, bool animation) :
        QLayout(parent)
    {
        _duration = 200;
        _verticalSpacing = 7;
        _horizontalSpacing = 7;
        this->setContentsMargins(11, 11, 11, 11);
        this->_enableAnimation = animation;
        this->_easing = QEasingCurve::Type::Linear;
    }

    void FlowLayout::addItem(QLayoutItem *item)
    {
        this->_items.append(item);
    }

    void FlowLayout::addWidget(QWidget *widget)
    {
        QLayout::addWidget(widget);
        if (this->_enableAnimation && widget)
        {
            auto *ani = new QPropertyAnimation(widget, "geometry");
            ani->setEndValue(QRect(QPoint(0, 0), widget->size()));
            ani->setDuration(this->_duration);
            ani->setEasingCurve(this->_easing);
            this->_widgetAnis.insert(widget, ani);
        }
    }

    int FlowLayout::count() const
    {
        return this->_items.size();
    }

    QLayoutItem *FlowLayout::itemAt(int index) const
    {
        return this->_items.value(index, nullptr);
    }

    QLayoutItem *FlowLayout::takeAt(int index)
    {
        if (index >= 0 && index < this->_items.size())
        {
            QLayoutItem *item = this->_items.takeAt(index);
            QWidget *widget = item->widget();
            if (QPropertyAnimation *ani = this->_widgetAnis.value(widget, nullptr); ani)
            {
                this->_widgetAnis.remove(widget);
                ani->deleteLater();
            }
            return item;
        }
        return nullptr;
    }

    void FlowLayout::setAnimationProperty(int msecs, const QEasingCurve &easing)
    {
        if (this->_enableAnimation)
        {
            this->_duration = msecs;
            this->_easing = easing;
            for (QPropertyAnimation *ani: std::as_const(this->_widgetAnis))
            {
                ani->setDuration(msecs);
                ani->setEasingCurve(easing);
            }
        }
    }

    void FlowLayout::removeWidget(QWidget *widget)
    {
        if (!widget)
            return;
        for (int i = 0; i < this->_items.size(); i++)
            if (this->_items[i]->widget() == widget)
                this->takeAt(i);
    }

    void FlowLayout::clearWidget()
    {
        while (!this->_items.isEmpty())
            this->takeAt(0);
    }

    Qt::Orientations FlowLayout::expandingDirections() const
    {
        return {};
    }

    bool FlowLayout::hasHeightForWidth() const
    {
        return true;
    }

    int FlowLayout::heightForWidth(int width) const
    {
        return this->doLayout({0, 0, width, 0}, true);
    }

    void FlowLayout::setGeometry(const QRect &rect)
    {
        QLayout::setGeometry(rect);
        // ReSharper disable once CppExpressionWithoutSideEffects
        doLayout(rect, false);
    }

    QSize FlowLayout::sizeHint() const
    {
        return this->maximumSize();
    }

    QSize FlowLayout::minimumSize() const
    {
        QSize size;
        for (const auto &item: this->_items)
            size = size.expandedTo(item->minimumSize());
        const QMargins margins = this->contentsMargins();
        size += {margins.left() + margins.right(), margins.top() + margins.bottom()};
        return size;
    }

    void FlowLayout::setVerticalSpacing(int spacing)
    {
        this->_verticalSpacing = spacing;
    }

    int FlowLayout::verticalSpacing() const
    {
        return this->_verticalSpacing;
    }

    void FlowLayout::setHorizontalSpacing(int spacing)
    {
        this->_horizontalSpacing = spacing;
    }

    int FlowLayout::horizontalSpacing() const
    {
        return this->_horizontalSpacing;
    }

    int FlowLayout::doLayout(const QRect &rect, bool count) const
    {
        const QMargins margins = this->contentsMargins();
        const QRect effectiveRect = rect.adjusted(margins.left(), margins.top(), margins.right(), margins.bottom());
        int x = effectiveRect.x();
        int y = effectiveRect.y();
        int rowHeight = 0;
        int spaceX = this->horizontalSpacing();
        int spaceY = this->verticalSpacing();
        for (auto item: this->_items)
        {
            int nextX = x + item->sizeHint().width() + spaceX;
            if (nextX > effectiveRect.right() && rowHeight > 0)
            {
                x = effectiveRect.x();
                y = y + rowHeight + spaceY;
                nextX = x + item->sizeHint().width() + spaceX;
                rowHeight = 0;
            }
            if (!count)
            {
                QRect target{{x, y}, item->sizeHint()};
                if (!this->_enableAnimation)
                    item->setGeometry(target);
                else if (auto ani = this->_widgetAnis.value(item->widget(), nullptr); ani)
                {
                    ani->stop();
                    ani->setEndValue(target);
                    ani->start();
                }
            }
            x = nextX;
            rowHeight = qMax(rowHeight, item->sizeHint().height());
        }
        return y + rowHeight + margins.bottom() - rect.y();
    }
} // fancy
