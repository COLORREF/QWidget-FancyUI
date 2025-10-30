//
// Created by TX on 2025/8/21.
//

#include "BezierEasing.h"


namespace fancy
{
    BezierEasing::BezierEasing(double x1, double y1, double x2, double y2) :
        _mX1(x1),
        _mY1(y1),
        _mX2(x2),
        _mY2(y2)
    {
        if (!(0.0 <= _mX1 && _mX1 <= 1.0 && 0.0 <= _mX2 && _mX2 <= 1.0))
        {
            qWarning() << "Bezier x values must be in [0, 1] range";
            return;
        }

        // 预计算采样表
        _sampleValues.resize(kSplineTableSize);
        if (_mX1 != _mY1 || _mX2 != _mY2)
        {
            for (int i = 0; i < kSplineTableSize; ++i)
                _sampleValues[i] = calcBezier(i * kSampleStepSize, _mX1, _mX2);
        }
    }

    double BezierEasing::getTForX(double x) const
    {
        double intervalStart = 0.0;
        int currentSample = 1;
        int lastSample = kSplineTableSize - 1;

        // 找到区间
        while (currentSample != lastSample && _sampleValues[currentSample] <= x)
        {
            intervalStart += kSampleStepSize;
            ++currentSample;
        }
        --currentSample;

        // 插值初始猜测值
        double dist = (x - _sampleValues[currentSample]) /
                      (_sampleValues[currentSample + 1] - _sampleValues[currentSample]);
        double guessT = intervalStart + dist * kSampleStepSize;

        // 判断是否用牛顿法，否则二分法
        double initialSlope = getSlope(guessT, _mX1, _mX2);
        if (initialSlope >= NEWTON_MIN_SLOPE)
            return newtonRaphsonIterate(x, guessT, _mX1, _mX2);
        if (initialSlope == 0.0)
            return guessT;
        return binarySubdivide(x, intervalStart, intervalStart + kSampleStepSize, _mX1, _mX2);
    }
} // fancy
