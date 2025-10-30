//
// Created by TX on 2025/10/3.
//

#ifndef QWIDGET_FANCYUI_PARALLELANIMATIONGROUPPOOL_H
#define QWIDGET_FANCYUI_PARALLELANIMATIONGROUPPOOL_H
#include <QParallelAnimationGroup>
#include <QQueue>
#include <QSet>

namespace fancy
{
    class ParallelAnimationGroupPool : public QObject
    {
        Q_OBJECT

    public:
        explicit ParallelAnimationGroupPool(const std::function<QParallelAnimationGroup*()> &factory, QObject *parent = nullptr);

        [[nodiscard]] int idleSize() const { return _idle.size(); } //空闲动画组数量
        [[nodiscard]] int runningSize() const { return _running.size(); } //运行动画组数量
        [[nodiscard]] bool isIdleEmpty() const { return _idle.isEmpty(); } //空闲动画组数量是否为空
        [[nodiscard]] bool isRunningEmpty() const { return _running.isEmpty(); } //运行动画组数量是否为空

        const QQueue<QParallelAnimationGroup *> &idle() { return _idle; };
        const QQueue<QParallelAnimationGroup *> &running() { return _running; };

        void clear(); //清空删除所有动画,请不要试图在动画运行时调用此函数

        void addGroup(QParallelAnimationGroup *group); //向空闲动画组队列添加一个动画组，会在该动画组finished信号发出时自动出运行队列进空闲队列

        bool stopAnimation(QParallelAnimationGroup *group) const; //在运行队列中查找并尝试停止指定的动画组，返回调用停止函数是否成功

        // 手动归还动画组到空闲队列中，调用 acquireGroup 手动运行动画组后若需要回归空闲队列请调用此函数，而不是调用 addGroup
        // 不要在调用 acquireGroupAndStart 函数自动运行动画后，将所运行的动画组调用此函数回归空闲队列
        void releaseGroup(QParallelAnimationGroup *group);

        // 取出空闲队列的第一个空闲动画组，调用此函数视为调用者管理该动画组的入队出队情况，将断开 finished 信号与自身的所有连接，运行时不会自动加入运行队列，运行结束也不会自动加入空闲队列
        // 若无空闲动画组则调用 _factory函数对象创建一个动画组
        // 若动画组由 _factory 函数新创建，那么运行时不会自动加入运行队列，运行结束也不会自动加入空闲队列
        // 若需要在运行结束后归还到空闲队列请调用 releaseGroup 而不是 addGroup
        QParallelAnimationGroup *acquireGroup();

        // 取出空闲队列的第一个空闲动画组加入到运行队列并开始运行，运行结束后自动加入到空闲队列移出运行队列，若无空闲动画组则调用 _factory 函数对象创建一个动画组，并返回所运行的动画组
        // 请不要在外部断开 finished 信号与 ParallelAnimationGroupPool 的连接
        QParallelAnimationGroup *acquireGroupAndStart();

    private:
        QSet<QParallelAnimationGroup *> _connectedGroups; // 记录已连接的动画组
        QQueue<QParallelAnimationGroup *> _idle;
        QQueue<QParallelAnimationGroup *> _running;
        std::function<QParallelAnimationGroup*()> _factory; // 指定一个函数用于创建动画组，在尝试取出一个动画组时若无空闲动画组则会调用此函数创建一个动画组;

        QParallelAnimationGroup *acquireGroupInternal(bool startImmediately);

    private slots:
        void handleAnimationFinished();
    };
} // fancy

#endif //QWIDGET_FANCYUI_PARALLELANIMATIONGROUPPOOL_H
