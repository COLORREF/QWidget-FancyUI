#ifndef FANCYLAYOUT_H
#define FANCYLAYOUT_H

#include <QLayout>
#include <QObject>
#include <QWidget>
#include <QVariant>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMap>

class FlowLayout : public QLayout
{
    Q_OBJECT
public:
    FlowLayout(QWidget *parent = nullptr,bool animation = true):
        QLayout(parent)
    {
        _duration = 200;
        _verticalSpacing=7;
        _horizontalSpacing=7;
        this->setContentsMargins(11,11,11,11);
        this->_enableAnimation = animation;
        this->_easing = QEasingCurve::Type::Linear;
    }
    void addItem(QLayoutItem* item)override
    {
        this->_items.append(item);
    }

    void addWidget(QWidget *widget)
    {
        QLayout::addWidget(widget);
        if(this->_enableAnimation && widget)
        {
            QPropertyAnimation* ani = new QPropertyAnimation(widget, "geometry");
            ani->setEndValue(QRect(QPoint(0,0),widget->size()));
            ani->setDuration(this->_duration);
            ani->setEasingCurve(this->_easing);
            this->_widgetAnis.insert(widget,ani);
        }
    }
    int count()const override
    {
        return this->_items.size();
    }
    QLayoutItem *itemAt(int index) const override
    {
        return this->_items.value(index,nullptr);
    }
    QLayoutItem *takeAt(int index)override
    {
        if (index >= 0 && index < this->_items.size())
        {
            QLayoutItem * item = this->_items.takeAt(index);
            QWidget* widget = item->widget();
            if(QPropertyAnimation* ani =this->_widgetAnis.value(widget,nullptr);ani)
            {
                this->_widgetAnis.remove(widget);
                ani->deleteLater();
            }
            return item;
        }
        return nullptr;
    }
    void setAnimationProperty(int msecs, const QEasingCurve &easing)
    {
        if(this->_enableAnimation)
        {
            this->_duration = msecs;
            this->_easing = easing;
            for(QPropertyAnimation* ani : std::as_const(this->_widgetAnis))
            {
                ani->setDuration(msecs);
                ani->setEasingCurve(easing);
            }
        }
    }
    void removeWidget(QWidget* widget)
    {
        if(!widget)return;
        for(int i = 0; i < this->_items.size();i++)
            if(this->_items[i]->widget() == widget)
                this->takeAt(i);
    }
    void clearWidget()
    {
        while(!this->_items.isEmpty())
            this->takeAt(0);
    }
    Qt::Orientations expandingDirections() const override
    {
        return {};
    }
    bool hasHeightForWidth()const override
    {
        return true;
    }
    int heightForWidth(int width)const override
    {
        return this->doLayout({0,0,width,0},true);
    }
    void setGeometry(const QRect &rect)override
    {
        QLayout::setGeometry(rect);
        doLayout(rect, false);
    }
    QSize sizeHint()const override
    {
        return this->maximumSize();
    }
    QSize minimumSize() const override
    {
        QSize size;
        for (const auto& item : this->_items)
            size = size.expandedTo(item->minimumSize());
        const QMargins margins = this->contentsMargins();
        size += {margins.left() + margins.right(), margins.top() + margins.bottom()};
        return size;
    }
    void setVerticalSpacing(int spacing)
    {
        this->_verticalSpacing = spacing;
    }
    int verticalSpacing()const
    {
        return this->_verticalSpacing;
    }
    void setHorizontalSpacing(int spacing)
    {
        this->_horizontalSpacing = spacing;
    }
    int horizontalSpacing()const
    {
        return this->_horizontalSpacing;
    }


private:
    int doLayout(const QRect& rect, bool count)const
    {
        QMargins margins = this->contentsMargins();
        QRect effectiveRect = rect.adjusted(margins.left(), margins.top(), margins.right(), margins.bottom());
        int x = effectiveRect.x();
        int y = effectiveRect.y();
        int rowHeight = 0;
        int spaceX = this->horizontalSpacing();
        int spaceY = this->verticalSpacing();
        for(auto item : this->_items)
        {
            int nextX = x + item->sizeHint().width() + spaceX;
            if(nextX > effectiveRect.right() && rowHeight > 0)
            {
                x = effectiveRect.x();
                y = y + rowHeight + spaceY;
                nextX = x + item->sizeHint().width() + spaceX;
                rowHeight = 0;
            }
            if(!count)
            {
                QRect target{{x,y},item->sizeHint()};
                if(!this->_enableAnimation)
                    item->setGeometry(target);
                else if(auto ani = this->_widgetAnis.value(item->widget(),nullptr);ani)
                {
                    ani->stop();
                    ani->setEndValue(target);
                    ani->start();
                }
            }
            x = nextX;
            rowHeight = qMax(rowHeight, item->sizeHint().height());
        }
        return  y + rowHeight + margins.bottom()- rect.y();
    }

    bool _enableAnimation;
    int _duration;
    int _verticalSpacing;
    int _horizontalSpacing;
    QList<QLayoutItem*> _items;
    QMap<QWidget*,QPropertyAnimation*> _widgetAnis;
    QEasingCurve _easing;//动画曲线
};

#endif // FANCYLAYOUT_H
