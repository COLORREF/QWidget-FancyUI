//
// Created by TX on 2025/8/21.
//

#ifndef QWIDGET_FANCYUI_BEZIEREASING_H
#define QWIDGET_FANCYUI_BEZIEREASING_H
#include <QDebug>
#include <QList>

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
            return calcBezier(getTForX(x), _mY1, _mY2);
        }

    private:
        static constexpr int NEWTON_ITERATIONS = 4; // 牛顿迭代次数
        static constexpr double NEWTON_MIN_SLOPE = 0.001; // 牛顿法最小斜率
        static constexpr double SUBDIVISION_PRECISION = 1e-7; // 二分法精度
        static constexpr int SUBDIVISION_MAX_ITERATIONS = 10; // 二分法最大迭代次数
        static constexpr int kSplineTableSize = 11; // 采样点数量
        static constexpr double kSampleStepSize = 1.0 / (kSplineTableSize - 1.0);

        double _mX1, _mY1, _mX2, _mY2;
        QList<double> _sampleValues;

        constexpr static double A(double a1, double a2) { return 1.0 - 3.0 * a2 + 3.0 * a1; }
        constexpr static double B(double a1, double a2) { return 3.0 * a2 - 6.0 * a1; }
        constexpr static double C(double a1) { return 3.0 * a1; }

        // 计算贝塞尔函数值 (x 或 y)
        constexpr static double calcBezier(double t, double a1, double a2)
        {
            return ((A(a1, a2) * t + B(a1, a2)) * t + C(a1)) * t;
        }

        // 计算导数 (斜率)
        constexpr static double getSlope(double t, double a1, double a2)
        {
            return 3.0 * A(a1, a2) * t * t + 2.0 * B(a1, a2) * t + C(a1);
        }

        // 二分法求解 t
        constexpr static double binarySubdivide(double x, double a, double b, double x1, double x2)
        {
            double currentT = 0.0, currentX = 0.0;
            for (int i = 0; i < SUBDIVISION_MAX_ITERATIONS; ++i)
            {
                currentT = a + (b - a) / 2.0;
                currentX = calcBezier(currentT, x1, x2) - x;
                if (std::fabs(currentX) < SUBDIVISION_PRECISION)
                    return currentT;
                if (currentX > 0.0)
                    b = currentT;
                else
                    a = currentT;
            }
            return currentT;
        }

        // 牛顿迭代求解 t
        constexpr static double newtonRaphsonIterate(double x, double guessT, double x1, double x2)
        {
            for (int i = 0; i < NEWTON_ITERATIONS; ++i)
            {
                double slope = getSlope(guessT, x1, x2);
                if (slope == 0.0)
                    return guessT;
                double currentX = calcBezier(guessT, x1, x2) - x;
                guessT -= currentX / slope;
            }
            return guessT;
        }

        // 根据 x 反求 t
        [[nodiscard]] double getTForX(double x) const;
    };
}

#endif //QWIDGET_FANCYUI_BEZIEREASING_H
