//
// Created by TX on 2025/8/21.
//

#include "BezierEasing.h"


#include <algorithm>
#include <cmath>

namespace fancy
{
    BezierEasing::BezierEasing(double x1, double y1, double x2, double y2) :
        _mX1(x1),
        _mY1(y1),
        _mX2(x2),
        _mY2(y2),
        _a((3.0 * x1 - 3.0 * x2 + 1.0) / 2.0),
        _b(x2 - 2.0 * x1),
        _c(x1),
        _ay(3.0 * y1 - 3.0 * y2 + 1.0),
        _by(3.0 * (y2 - 2.0 * y1)),
        _cy(3.0 * y1)
    {
        if (!(0.0 <= _mX1 && _mX1 <= 1.0 && 0.0 <= _mX2 && _mX2 <= 1.0))
            qWarning() << "Bezier x values must be in [0, 1] range";
    }

    // Solves x(t) = ((2a * t + 3b) * t + 3c) * t = x for t, with x in (0, 1):
    // u = 1/t is the largest real root of x·u³ − 3c·u² − 3b·u − 2a = 0
    double BezierEasing::solveTForX(double x, double a, double b, double c)
    {
        const double j = 1.0 / std::max(c, std::sqrt(x));
        const double k = x * j;
        const double l = k * j;
        const double s = c * j;
        const double q = b * l;
        const double m = s * s + q;
        const double h = -s * (s * s + 1.5 * q) - a * k * l;
        const double D = h * h - m * m * m;
        double v;
        if (m == 0.0 || D > 1e-12 * h * h)
        {
            // one real root (Cardano)
            const double U = -std::cbrt(h < 0.0 ? h - std::sqrt(D) : h + std::sqrt(D));
            v = U + m / U;
            if (std::isnan(v)) // triple root (m = h = 0)
                v = 0.0;
        }
        else
        {
            // three real roots, take the largest
            const double r = std::sqrt(m);
            v = 2.0 * r * std::cos(std::acos(std::clamp(-h / (m * r), -1.0, 1.0)) / 3.0);
        }
        return std::min(1.0, k / (v + s));
    }
} // fancy
