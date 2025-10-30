//
// Created by TX on 2025/10/3.
//

#include "ParallelAnimationGroupPool.h"

namespace fancy
{
    ParallelAnimationGroupPool::ParallelAnimationGroupPool(const std::function<QParallelAnimationGroup *()> &factory, QObject *parent) :
        QObject(parent),
        _factory(factory) {}

    void ParallelAnimationGroupPool::clear()
    {
        qDeleteAll(_idle);
        qDeleteAll(_running);
        _idle.clear();
        _running.clear();
    }

    void ParallelAnimationGroupPool::addGroup(QParallelAnimationGroup *group)
    {
        _idle.enqueue(group);
        connect(group, &QParallelAnimationGroup::finished, this, &ParallelAnimationGroupPool::handleAnimationFinished);
        _connectedGroups.insert(group); //记录连接
    }

    bool ParallelAnimationGroupPool::stopAnimation(QParallelAnimationGroup *group) const
    {
        int index = _running.indexOf(group);
        if (index == -1)
            return false;
        group->stop();
        return true;
    }

    void ParallelAnimationGroupPool::releaseGroup(QParallelAnimationGroup *group)
    {
        _idle.enqueue(group);
    }

    QParallelAnimationGroup *ParallelAnimationGroupPool::acquireGroup()
    {
        return acquireGroupInternal(false);
    }

    QParallelAnimationGroup *ParallelAnimationGroupPool::acquireGroupAndStart()
    {
        return acquireGroupInternal(true);
    }

    QParallelAnimationGroup *ParallelAnimationGroupPool::acquireGroupInternal(bool startImmediately)
    {
        QParallelAnimationGroup *group;
        if (_idle.isEmpty())
            group = _factory();
        else
            group = _idle.dequeue();

        if (startImmediately) //立即开始
        {
            if (!_connectedGroups.contains(group)) //未连接
            {
                connect(group, &QParallelAnimationGroup::finished, this, &ParallelAnimationGroupPool::handleAnimationFinished);
                _connectedGroups.insert(group); //记录连接
            }
            _running.enqueue(group);
            group->start();
        }
        else
        {
            if (_connectedGroups.contains(group)) //已连接
            {
                disconnect(group, &QParallelAnimationGroup::finished, this, &ParallelAnimationGroupPool::handleAnimationFinished); //断开连接
                _connectedGroups.remove(group);
            }
        }
        return group;
    }

    void ParallelAnimationGroupPool::handleAnimationFinished()
    {
        // _running.dequeue();
        _running.removeOne(static_cast<QParallelAnimationGroup *>(sender()));
        _idle.enqueue(static_cast<QParallelAnimationGroup *>(sender()));
    }
} // fancy
