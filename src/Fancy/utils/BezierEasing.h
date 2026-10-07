//
// Created by TX on 2025/8/21.
//

#ifndef QWIDGET_FANCYUI_BEZIEREASING_H
#define QWIDGET_FANCYUI_BEZIEREASING_H
#include <QDebug>

namespace fancy
{
    class BezierEasing
    {
    public:
        // 构造函数，输入四个控制点 (x1, y1, x2, y2)
        BezierEasing(double x1, double y1, double x2, double y2);

        // 计算贝塞尔缓动函数值 (输入: x ∈ [0,1], 输出: y ∈ [0,1])
        double operator()(double x) const
        {
            if (_mX1 == _mY1 && _mX2 == _mY2)
                return x; // 线性情况
            if (x <= 0.0)
                return 0.0;
            if (x >= 1.0)
                return 1.0;
            if (x != x)
                return x; // NaN
            // x(t) = ((2a * t + 3b) * t + 3c) * t, y(t) = ((ay * t + by) * t + cy) * t
            const double t = solveTForX(x, _a, _b, _c);
            return ((_ay * t + _by) * t + _cy) * t;
        }

    private:
        double _mX1, _mY1, _mX2, _mY2;
        double _a, _b, _c, _ay, _by, _cy;

        // 根据 x 反求 t (closed-form solver from https://github.com/gre/bezier-easing)
        static double solveTForX(double x, double a, double b, double c);
    };
}

#endif //QWIDGET_FANCYUI_BEZIEREASING_H
