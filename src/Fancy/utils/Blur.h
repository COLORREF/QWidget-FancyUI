//
// Created by A1840 on 2026/8/31.
//

#ifndef QWIDGET_FANCYUI_IMAGEPROCESS_H
#define QWIDGET_FANCYUI_IMAGEPROCESS_H
#include <QImage>
#include <QtMath>

namespace fancy
{
    inline QImage gaussianBlur(const QImage &src, const int radius)
    {
        if (radius <= 0 || src.isNull())
            return src;

        const int width = src.width();
        const int height = src.height();
        const int kernelSize = 2 * radius + 1;
        const qreal sigma = 0.3 * ((kernelSize - 1) * 0.5 - 1) + 0.8;

        // 生成高斯核
        QVector<qreal> kernel(kernelSize);
        qreal sum = 0.0;
        for (int i = 0; i < kernelSize; ++i)
        {
            const qreal x = i - radius;
            const qreal g = qExp(-(x * x) / (2.0 * sigma * sigma));
            kernel[i] = g;
            sum += g;
        }
        for (auto &v: kernel)
            v /= sum;

        QImage temp(src.size(), QImage::Format_ARGB32);
        QImage dst(src.size(), QImage::Format_ARGB32);

        const int bytesPerLine = static_cast<int>(src.bytesPerLine());
        const uchar *srcBits = src.constBits();
        uchar *tempBits = temp.bits();
        uchar *dstBits = dst.bits();

        // -------- 横向模糊 --------
        for (int y = 0; y < height; ++y)
        {
            const uchar *srcLine = srcBits + y * bytesPerLine;
            uchar *tempLine = tempBits + y * bytesPerLine;

            for (int x = 0; x < width; ++x)
            {
                double rSum = 0, gSum = 0, bSum = 0, aSum = 0;

                for (int k = -radius; k <= radius; ++k)
                {
                    const int px = std::clamp(x + k, 0, width - 1);
                    const uchar *p = srcLine + px * 4; // RGBA
                    const double w = kernel[k + radius];

                    bSum += p[0] * w;
                    gSum += p[1] * w;
                    rSum += p[2] * w;
                    aSum += p[3] * w;
                }

                uchar *d = tempLine + x * 4;
                d[0] = static_cast<uchar>(std::lround(bSum));
                d[1] = static_cast<uchar>(std::lround(gSum));
                d[2] = static_cast<uchar>(std::lround(rSum));
                d[3] = static_cast<uchar>(std::lround(aSum));
            }
        }

        // -------- 纵向模糊 --------
        for (int y = 0; y < height; ++y)
        {
            uchar *dstLine = dstBits + y * bytesPerLine;

            for (int x = 0; x < width; ++x)
            {
                double rSum = 0, gSum = 0, bSum = 0, aSum = 0;

                for (int k = -radius; k <= radius; ++k)
                {
                    const int py = std::clamp(y + k, 0, height - 1);
                    const uchar *p = tempBits + py * bytesPerLine + x * 4;
                    const double w = kernel[k + radius];

                    bSum += p[0] * w;
                    gSum += p[1] * w;
                    rSum += p[2] * w;
                    aSum += p[3] * w;
                }

                uchar *d = dstLine + x * 4;
                d[0] = static_cast<uchar>(std::lround(bSum));
                d[1] = static_cast<uchar>(std::lround(gSum));
                d[2] = static_cast<uchar>(std::lround(rSum));
                d[3] = static_cast<uchar>(std::lround(aSum));
            }
        }

        return dst;
    }

    inline QImage boxBlur(const QImage &src, const int kernelSize)
    {
        if (kernelSize <= 1 || src.isNull())
            return src;

        const int width = src.width();
        const int height = src.height();
        const int radius = (kernelSize - 1) / 2;

        QImage temp(src.size(), QImage::Format_ARGB32);
        QImage dst(src.size(), QImage::Format_ARGB32);

        const int srcBpl = static_cast<int>(src.bytesPerLine());
        const int tempBpl = static_cast<int>(temp.bytesPerLine());
        const int dstBpl = static_cast<int>(dst.bytesPerLine());
        const uchar *srcBits = src.constBits();
        uchar *tempBits = temp.bits();
        uchar *dstBits = dst.bits();

        const double norm = 1.0 / kernelSize;

        // ── 横向滑动窗口 ──
        for (int y = 0; y < height; ++y)
        {
            const uchar *srcLine = srcBits + y * srcBpl;
            uchar *tempLine = tempBits + y * tempBpl;

            // 初始化窗口：[−radius, radius] → clamp 到 [0, width−1]
            double rSum = 0, gSum = 0, bSum = 0, aSum = 0;
            for (int k = -radius; k <= radius; ++k)
            {
                const int px = std::clamp(k, 0, width - 1);
                const uchar *p = srcLine + px * 4;
                bSum += p[0];
                gSum += p[1];
                rSum += p[2];
                aSum += p[3];
            }

            for (int x = 0; x < width; ++x)
            {
                uchar *d = tempLine + x * 4;
                d[0] = static_cast<uchar>(std::lround(bSum * norm));
                d[1] = static_cast<uchar>(std::lround(gSum * norm));
                d[2] = static_cast<uchar>(std::lround(rSum * norm));
                d[3] = static_cast<uchar>(std::lround(aSum * norm));

                // 滑动：减去最左像素，加入最右新像素
                const int leftX = std::clamp(x - radius, 0, width - 1);
                const int rightX = std::clamp(x + radius + 1, 0, width - 1);
                const uchar *lp = srcLine + leftX * 4;
                const uchar *rp = srcLine + rightX * 4;
                bSum -= lp[0];
                bSum += rp[0];
                gSum -= lp[1];
                gSum += rp[1];
                rSum -= lp[2];
                rSum += rp[2];
                aSum -= lp[3];
                aSum += rp[3];
            }
        }

        // ── 纵向滑动窗口 ──
        for (int x = 0; x < width; ++x)
        {
            // 初始化窗口
            double rSum = 0, gSum = 0, bSum = 0, aSum = 0;
            for (int k = -radius; k <= radius; ++k)
            {
                const int py = std::clamp(k, 0, height - 1);
                const uchar *p = tempBits + py * tempBpl + x * 4;
                bSum += p[0];
                gSum += p[1];
                rSum += p[2];
                aSum += p[3];
            }

            for (int y = 0; y < height; ++y)
            {
                uchar *d = dstBits + y * dstBpl + x * 4;
                d[0] = static_cast<uchar>(std::lround(bSum * norm));
                d[1] = static_cast<uchar>(std::lround(gSum * norm));
                d[2] = static_cast<uchar>(std::lround(rSum * norm));
                d[3] = static_cast<uchar>(std::lround(aSum * norm));

                const int topY = std::clamp(y - radius, 0, height - 1);
                const int bottomY = std::clamp(y + radius + 1, 0, height - 1);
                const uchar *tp = tempBits + topY * tempBpl + x * 4;
                const uchar *bp = tempBits + bottomY * tempBpl + x * 4;
                bSum -= tp[0];
                bSum += bp[0];
                gSum -= tp[1];
                gSum += bp[1];
                rSum -= tp[2];
                rSum += bp[2];
                aSum -= tp[3];
                aSum += bp[3];
            }
        }

        return dst;
    }

    inline QImage approxGaussianBlur(const QImage &src, const int radius)
    {
        if (radius <= 0)
            return src;
        const int ksize = radius * 2 + 1;
        return boxBlur(boxBlur(boxBlur(src, ksize), ksize), ksize);;
    }

}

#endif //QWIDGET_FANCYUI_IMAGEPROCESS_H
