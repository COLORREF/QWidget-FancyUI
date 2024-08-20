#ifndef ANIMATION_H
#define ANIMATION_H

#include <QVariantAnimation>
#include <QWidget>
#include <QObject>
#include <QWidget>
#include <QQueue>
#include <QMetaMethod>
#include <QParallelAnimationGroup>
#include <QSet>
#include <QPainter>
#include "theme.h"
#include "precompile.h"

#ifdef _WIN32
class ThemeColorManagement : public QVariantAnimation
{
    Q_OBJECT
public:
    explicit ThemeColorManagement(QObject *parent = nullptr, const QColor &light = QColor(240, 243, 249), const QColor &dark = QColor(26,32,52));
    void setThemeColor(const QColor &light,const QColor &dark);
    QColor lightColor();
    QColor darkColor();
    bool isEnable();

    F_PUBLIC_PORINTER_GET(QColor,runTimeColor)
    F_PROTECTED_PROPERTY(QColor,runTimeColor)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(bool,enabled,Enable,)
private slots:
    void updateRunTimeColor(const QVariant &value);
    void onThemeChange(Theme::Type type);
};
#endif

class SimpleAnimation : public QVariantAnimation
{
    Q_OBJECT
public:
    SimpleAnimation(const QVariant &start, const QVariant &end, int time_msecs, bool isforward, QObject *parent = nullptr)
        : QVariantAnimation(parent)
    {
        this->setStartValue(start);
        this->setEndValue(end);
        this->setDuration(time_msecs);
        this->setDirection(isforward);
        connect(this,&QVariantAnimation::valueChanged,this,[&](const QVariant &value){_runTimeValue = value;});
    }

    using QVariantAnimation::setDirection;
    inline void setUpdate(QWidget *device)
    {
        connect(this, &QVariantAnimation::valueChanged, device, QOverload<>::of(&QWidget::update));
        // connect(this,&QVariantAnimation::valueChanged,device,static_cast<void(QWidget::*)()>(&QWidget::update));
    };

    inline virtual void paint(QPainter* painter,void* extra = nullptr)
    {
        Q_UNUSED(painter);
        Q_UNUSED(extra);
    }
    inline void setValue(const QVariant& start, const QVariant& end){setStartValue(start);setEndValue(end);}
    mutable QVariant _runTimeValue;
public slots:
    inline void setDirection(bool isforward = true) { QVariantAnimation::setDirection((Direction)!isforward); };
    inline void setForward() { setDirection(Direction::Forward); }
    inline void setBackward() { setDirection(Direction::Backward); }
    inline void reverseDirectionAndStartWithPolicy(QAbstractAnimation::DeletionPolicy policy = KeepWhenStopped)
    {
        reverseDirection();
        start(policy);
    };
    inline void reverseDirectionAndStart()
    {
        reverseDirection();
        start();
    };
    inline void reverseDirection() { QVariantAnimation::setDirection((Direction)!direction());}
};


// 并行动画组池
class ParallelAnimationGroupPool : public QObject
{
    Q_OBJECT
public:
    ParallelAnimationGroupPool(std::function<QParallelAnimationGroup*(void)> factory, QObject *parent = nullptr):
        QObject(parent)
    {
        _factory = factory;
    }

    inline int idleSize()const{return _idle.size();}//空闲动画组数量
    inline int runningSize()const{return _running.size();};//运行动画组数量
    inline bool isIdleEmpty() const{return _idle.isEmpty();}//空闲动画组数量是否为空
    inline bool isrunningEmpty()const{return _running.isEmpty();};//运行动画组数量是否为空
    inline void clear(){qDeleteAll(_idle);qDeleteAll(_running);_idle.clear();_running.clear();};//清空删除所有动画,请不要试图在动画运行时调用此函数
    inline void addGroup(QParallelAnimationGroup* group)//向空闲动画组队列添加一个动画组，会在该动画组finished信号发出时自动出运行队列进空闲队列
    {
        _idle.enqueue(group);
        connect(group, &QParallelAnimationGroup::finished, this, &ParallelAnimationGroupPool::handleAnimationFinished);
        _connectedGroups.insert(group);//记录连接

    }

    bool stopAnimation(QParallelAnimationGroup* group)const//在运行队列中查找并尝试停止指定的动画组，返回调用停止函数是否成功
    {
        int index = _running.indexOf(group);
        if(index==-1)
            return false;
        group->stop();
        return true;
    }

    //手动归还动画组到空闲队列中，调用acquireGrop手动运行动画组后若需要回归空闲队列请调用此函数，而不是调用addGroup
    //不要在调用acquireGropAndStart函数自动运行动画后，将所运行的动画组调用此函数回归空闲队列
    inline void releaseGroup(QParallelAnimationGroup* group){_idle.enqueue(group);}


    // 取出空闲队列的第一个空闲动画组，调用此函数视为调用者管理该动画组的入队出队情况，将断开finished信号与自身的所有连接，运行时不会自动加入运行队列，运行结束也不会自动加入空闲队列
    // 若无空闲动画组则调用_factory函数对象创建一个动画组
    // 若动画组由_factory函数新创建，那么运行时不会自动加入运行队列，运行结束也不会自动加入空闲队列
    // 若需要在运行结束后归还到空闲队列请调用releaseGroup而不是addGroup
    QParallelAnimationGroup* acquireGroup()
    {
        return acquireGroupInternal(false);
    }

    // 取出空闲队列的第一个空闲动画组加入到运行队列并开始运行，运行结束后自动加入到空闲队列移出运行队列，若无空闲动画组则调用_factory函数对象创建一个动画组，并返回所运行的动画组
    // 请不要在外部断开finished信号与ParallelAnimationGroupPool的连接
    QParallelAnimationGroup* acquireGropAndStart()
    {
        return acquireGroupInternal(true);
    }

    inline const QQueue<QParallelAnimationGroup*>& idle(){return _idle;};
    inline const QQueue<QParallelAnimationGroup*>& running(){return _running;};
private:
    QSet<QParallelAnimationGroup*> _connectedGroups; // 记录已连接的动画组
    QQueue<QParallelAnimationGroup*> _idle;
    QQueue<QParallelAnimationGroup*> _running;
    std::function<QParallelAnimationGroup*(void)> _factory;// 指定一个函数用于创建动画组，在尝试取出一个动画组时若无空闲动画组则会调用此函数创建一个动画组;
    QParallelAnimationGroup* acquireGroupInternal(bool startImmediately)
    {
        QParallelAnimationGroup* group;
        if (_idle.isEmpty())
            group = _factory();
        else
            group = _idle.dequeue();

        if(startImmediately)//立即开始
        {
            if (!_connectedGroups.contains(group))//未连接
            {
                connect(group, &QParallelAnimationGroup::finished, this, &ParallelAnimationGroupPool::handleAnimationFinished);
                _connectedGroups.insert(group);//记录连接
            }
            _running.enqueue(group);
            group->start();
        }
        else
        {
            if (_connectedGroups.contains(group))//已连接
            {
                disconnect(group, &QParallelAnimationGroup::finished, this, &ParallelAnimationGroupPool::handleAnimationFinished);//断开连接
                _connectedGroups.remove(group);
            }
        }
        return group;
    }
private slots:
    void handleAnimationFinished()
    {
        _running.dequeue();
        _idle.enqueue((QParallelAnimationGroup*)(sender()));
    }
};


// 点击波纹动画
class ClickRippleAnimation : public QObject
{
    Q_OBJECT
public:
    ClickRippleAnimation(int msecs, int maxRadius,const QColor& startColor,const QColor& endColor,quint32 initialQuantity, QObject* parent)
        :QObject(parent)
    {
        auto factory = [=]()->QParallelAnimationGroup*{
            QParallelAnimationGroup* group = new QParallelAnimationGroup(parent);
            SimpleAnimation* r_ani = new SimpleAnimation(0,maxRadius,msecs,true,group);
            SimpleAnimation* h_ani =  new SimpleAnimation(startColor,endColor,msecs,true,group);
            r_ani->_runTimeValue = 0;
            h_ani->_runTimeValue = QColor(Qt::transparent);
            group->addAnimation(r_ani);
            group->addAnimation(h_ani);
            connect(group,&QParallelAnimationGroup::finished,this,[this](){this->_paintPoint.dequeue();});
            return group;
        };

        _pool = new ParallelAnimationGroupPool(factory,this);
        for(quint32 i = 0; i < initialQuantity; i++)
            _pool->addGroup(factory());
    }
    void setUpdate(QWidget* device)
    {
        for(QParallelAnimationGroup* grop : _pool->idle())
        {
            ((SimpleAnimation*)(grop->animationAt(0)))->setUpdate(device);
            ((SimpleAnimation*)(grop->animationAt(1)))->setUpdate(device);
        }
    }
    void paint(QPainter* painter)
    {
        painter->save();
        const QQueue<QParallelAnimationGroup*>& groups = _pool->running();
        for(QParallelAnimationGroup* grop : groups)
        {
            painter->setBrush(((SimpleAnimation*)(grop->animationAt(1)))->_runTimeValue.value<QColor>());
            int r = ((SimpleAnimation*)(grop->animationAt(0)))->_runTimeValue.toInt();
            painter->drawEllipse(_paintPoint.at(groups.indexOf(grop)),r,r);
        }
        painter->restore();
    }
public slots:
    void updateMaxRadius(int maxRadius)
    {
        for(QParallelAnimationGroup* grop : _pool->idle())
            ((SimpleAnimation*)(grop->animationAt(0)))->setEndValue(maxRadius);
    }

    void updateStartColor(const QColor& startColor)
    {
        for(QParallelAnimationGroup* grop : _pool->idle())
            ((SimpleAnimation*)(grop->animationAt(1)))->setStartValue(startColor);
    }
    void updateEndColor(const QColor& endColor)
    {
        for(QParallelAnimationGroup* grop : _pool->idle())
            ((SimpleAnimation*)(grop->animationAt(1)))->setEndValue(endColor);
    }
    void start(QPointF pt)
    {
        _paintPoint.enqueue(pt);
        _pool->acquireGropAndStart();
    }
private:
    ParallelAnimationGroupPool* _pool;
    QQueue<QPointF> _paintPoint;// 绘制坐标
};


#endif // ANIMATION_H
