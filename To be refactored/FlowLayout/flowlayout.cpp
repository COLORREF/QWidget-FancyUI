#include "flowlayout.h"

FlowLayout::FlowLayout(QWidget *parent, bool needAni, bool isTight):
    QLayout::QLayout{parent}
{
    this->needAni = needAni;
    this->isTight = isTight;
}

void FlowLayout::addItem(QLayoutItem *item)
{
    this->items.append(item);
}

void FlowLayout::addWidget(QWidget *w)
{
    QLayout::addWidget(w);
    if(! this->needAni)
        return;
    QPropertyAnimation* ani = new QPropertyAnimation(w, "geometry");
    ani->setEndValue(QRect(QPoint(0,0),w->size()));
    ani->setDuration(this->duration);
    ani->setEasingCurve(this->ease);

    w->setProperty("flowAni", (qint64)ani);

    this->anis.append(ani);
    this->aniGroup->addAnimation(ani);
}

void FlowLayout::setAnimation(int duration, const QEasingCurve &ease)
{
    if(! this->needAni)
        return;
    this->duration = duration;
    this->ease = ease;

    for(auto& ani : this->anis)
    {
        ani->setDuration(duration);
        ani->setEasingCurve(ease);
    }
}

int FlowLayout::count() const
{
    return this->items.count();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    if(0 <= index && index < this->items.count())
        return this->items[index];

    return nullptr;
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if(0 <= index && index < this->items.count())
    {
        QLayoutItem * item = this->items[index];
        QPropertyAnimation* ani = (QPropertyAnimation*)(item->widget()->property("flowAni").value<qint64>());
        if(ani != nullptr)
        {
            this->anis.removeAt(this->anis.indexOf(ani));
            this->aniGroup->removeAnimation(ani);
            ani->deleteLater();
        }
        this->items.removeAt(index);
        return item;
    }
    return nullptr;
}

QWidget* FlowLayout::removeWidget(QWidget *widget)
{
    int index = 0;
    for(auto& item : this->items)
    {
        if(item->widget() == widget)
            return this->takeAt(index)->widget();
        index++;
    }
    return nullptr;
}

void FlowLayout::removeAllWidgets()
{
    while(!this->items.isEmpty())
        this->takeAt(0);
}

void FlowLayout::takeAllWidgets()
{
    while(!this->items.isEmpty())
    {
        QWidget* w = this->takeAt(0)->widget();
        if(w!=nullptr)
            w->deleteLater();
    }
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return Qt::Orientation(0x0);
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    return this->doLayout(QRect(0,0,width,0),false);
}

void FlowLayout::setGeometry(const QRect & rect)
{
    QLayout::setGeometry(rect);
    this->doLayout(rect,true);
}

QSize FlowLayout::sizeHint() const
{
    return this->minimumSize();
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    for(auto& item : this->items)
        size = size.expandedTo(item->minimumSize());
    QMargins m = contentsMargins();
    size += QSize(m.left()+m.right(), m.top()+m.bottom());
    return size;
}

void FlowLayout::setVerticalSpacing(int spacing)
{
    this->verticalSpacing_ = spacing;
}

int FlowLayout::verticalSpacing() const
{
    return this->verticalSpacing_;
}

void FlowLayout::setHorizontalSpacing(int spacing)
{
    this->horizontalSpacing_ = spacing;
}

int FlowLayout::horizontalSpacing() const
{
    return this->horizontalSpacing_;
}

int FlowLayout::doLayout(const QRect &rect, bool move)const
{
    bool aniRestart = false;
    QMargins margin = contentsMargins();
    int x = rect.x() + margin.left();
    int y = rect.y() + margin.top();
    int rowHeight = 0;
    int spaceX = horizontalSpacing();
    int spaceY = verticalSpacing();

    int index = 0;
    for(auto& item:this->items)
    {
        if(item->widget()!=nullptr && !item->widget()->isVisible() && this->isTight)
            continue;

        int nextX = x + item->sizeHint().width() + spaceX;
        if(nextX - spaceX > rect.right() && rowHeight > 0)
        {
            x = rect.x() + margin.left();
            y = y+ rowHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            rowHeight = 0;
        }
        if(move)
        {
            QRect target(QPoint(x,y),item->sizeHint());
            if(!this->needAni)
                item->setGeometry(target);
            else if(target != this->anis[index]->endValue())
            {
                this->anis[index]->stop();
                this->anis[index]->setEndValue(target);
                aniRestart = true;
            }
        }
        x = nextX;
        rowHeight = qMax<int,int>(rowHeight,item->sizeHint().height());
        index++;
    }
    if(this->needAni && aniRestart)
    {
        this->aniGroup->stop();
        this->aniGroup->start();
    }
    return y+rowHeight+margin.bottom()-rect.y();
}

