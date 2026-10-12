#include "IndicatorAnimation.h"

namespace fancy
{
    IndicatorAnimation::IndicatorAnimation(QObject *parent) :
        QVariantAnimation(parent),
        _reveal(QEasingCurve::BezierSpline),
        _stretch(QEasingCurve::BezierSpline),
        _settle(QEasingCurve::BezierSpline)
    {
        setStartValue(0.0);
        setEndValue(1.0);
        _reveal.addCubicBezierSegment({0, 0}, {0, 1}, {1, 1});
        _stretch.addCubicBezierSegment({0.9, 0.1}, {1, 0.2}, {1, 1});
        _settle.addCubicBezierSegment({0.1, 0.9}, {0.2, 1}, {1, 1});
        setEasingCurve(_reveal);
    }

    void IndicatorAnimation::reveal(bool animated, qreal initialLength, int msecs)
    {
        stop();
        _mode = Mode::Reveal;
        _initialLength = qMax(0.0, initialLength);
        setDuration(qMax(0, msecs));
        setEasingCurve(_reveal);
        if (animated)
            start();
        else
            setCurrentTime(duration());
    }

    qreal IndicatorAnimation::opacity() const
    {
        return _mode == Mode::Slide || duration() == 0 ? 1.0 : currentValue().toReal();
    }

    void IndicatorAnimation::slide(const QLineF &from, const QLineF &to, Qt::Orientation orientation, int msecs)
    {
        stop();
        _mode = Mode::Slide;
        _orientation = orientation;
        const auto normalized = [orientation](const QLineF &line) {
            const bool reversed = orientation == Qt::Horizontal ? line.x1() > line.x2() : line.y1() > line.y2();
            return reversed ? QLineF(line.p2(), line.p1()) : line;
        };
        _from = normalized(from);
        _to = normalized(to);
        setDuration(qMax(0, msecs));
        setEasingCurve(QEasingCurve::Linear);
        start();
    }

    QLineF IndicatorAnimation::geometry(const QLineF &target) const
    {
        if (_mode == Mode::Slide)
        {
            if (duration() == 0)
                return _to;
            const qreal time = currentTime() / static_cast<qreal>(duration());
            constexpr qreal split = 0.333;
            const QLineF middle = _orientation == Qt::Horizontal
                                      ? QLineF(qMin(_from.x1(), _to.x1()), _from.y1(), qMax(_from.x2(), _to.x2()), _from.y1())
                                      : QLineF(_from.x1(), qMin(_from.y1(), _to.y1()), _from.x1(), qMax(_from.y2(), _to.y2()));
            const bool expanding = time < split;
            const QLineF a = expanding ? _from : middle;
            const QLineF b = expanding ? middle : _to;
            const qreal progress = expanding ? _stretch.valueForProgress(time / split) : _settle.valueForProgress((time - split) / (1 - split));
            return {a.p1() + (b.p1() - a.p1()) * progress, a.p2() + (b.p2() - a.p2()) * progress};
        }
        // 沿目标方向展开
        const qreal length = target.length();
        if (qFuzzyIsNull(length))
            return target;
        const qreal initialLength = qMin(_initialLength, length);
        const qreal halfLength = (initialLength + (length - initialLength) * opacity()) / 2;
        const QPointF halfVector = (target.p2() - target.p1()) * (halfLength / length);
        const QPointF center = target.center();
        return {center - halfVector, center + halfVector};
    }
}
