#ifndef MINI_BLUR_QIMAGE_H
#define MINI_BLUR_QIMAGE_H

#include "mini_blur.h"
#include <QColorSpace>
#include <QImage>
#include <stdexcept>

namespace mini_blur
{
    namespace detail
    {
        inline int channelsFor(QImage::Format format)
        {
            switch (format)
            {
                case QImage::Format_Grayscale8: return 1;
                case QImage::Format_RGB888: return 3;
                case QImage::Format_RGB32:
                case QImage::Format_ARGB32:
                case QImage::Format_ARGB32_Premultiplied:
                case QImage::Format_RGBA8888:
                case QImage::Format_RGBA8888_Premultiplied: return 4;
                default: return 0;
            }
        }

        inline QImage supportedImage(const QImage &image)
        {
            if (image.isNull())
                throw std::invalid_argument("mini_blur: empty QImage");
            if (channelsFor(image.format()))
                return image;
            // Converts indexed and 16-bit formats to straight RGBA.
            return image.convertToFormat(QImage::Format_RGBA8888);
        }

        inline ImageView view(const QImage &image)
        {
            return {
                image.constBits(),
                image.width(),
                image.height(),
                channelsFor(image.format()),
                static_cast<std::size_t>(image.bytesPerLine())
            };
        }

        inline MutableImageView mutableView(QImage &image)
        {
            return {
                image.bits(),
                image.width(),
                image.height(),
                channelsFor(image.format()),
                static_cast<std::size_t>(image.bytesPerLine())
            };
        }

        inline void finishImage(const QImage &source, QImage &output, Border border)
        {
            // RGB32 requires an opaque alpha byte, even with zero border extension.
            if (output.format() == QImage::Format_RGB32 && border == Border::ConstantZero)
                for (int y = 0; y < output.height(); ++y)
                {
                    auto *row = reinterpret_cast<QRgb *>(output.scanLine(y));
                    for (int x = 0; x < output.width(); ++x)
                        row[x] |= 0xff000000u;
                }
            output.setDevicePixelRatio(source.devicePixelRatio());
            output.setDotsPerMeterX(source.dotsPerMeterX());
            output.setDotsPerMeterY(source.dotsPerMeterY());
            output.setColorSpace(source.colorSpace());
        }
    } // namespace detail

    // High-frequency API: reuses workers, scratch storage and a detached output
    // image of matching size/format. Input/output may be the same QImage.
    inline void gaussianBlurTo(
        Context &context,
        const QImage &input,
        QImage &output,
        int kw,
        int kh,
        double sx = 0,
        double sy = 0,
        Border border = Border::Reflect101)
    {
        const QImage source = detail::supportedImage(input);
        if (output.size() != source.size() || output.format() != source.format())
            output = QImage(source.size(), source.format());
        if (output.isNull())
            throw std::bad_alloc();
        context.gaussianBlur(detail::view(source), detail::mutableView(output), kw, kh, sx, sy, border);
        detail::finishImage(source, output, border);
    }

    inline void boxBlurTo(
        Context &context,
        const QImage &input,
        QImage &output,
        int kw,
        int kh,
        Border border = Border::Reflect101)
    {
        const QImage source = detail::supportedImage(input);
        if (output.size() != source.size() || output.format() != source.format())
            output = QImage(source.size(), source.format());
        if (output.isNull())
            throw std::bad_alloc();
        context.boxBlur(detail::view(source), detail::mutableView(output), kw, kh, border);
        detail::finishImage(source, output, border);
    }

    // Returns a new QImage. A supported input format is retained; other formats
    // are converted to RGBA8888. Alpha is filtered just like the color channels.
    inline QImage gaussianBlur(
        const QImage &input,
        int kernelWidth,
        int kernelHeight,
        double sigmaX = 0,
        double sigmaY = 0,
        Border border = Border::Reflect101)
    {
        const QImage source = detail::supportedImage(input);
        QImage output(source.size(), source.format());
        if (output.isNull())
            throw std::bad_alloc();
        gaussianBlur(detail::view(source), detail::mutableView(output), kernelWidth, kernelHeight,
                     sigmaX, sigmaY, border);
        detail::finishImage(source, output, border);
        return output;
    }

    inline QImage boxBlur(const QImage &input, int kernelWidth, int kernelHeight, Border border = Border::Reflect101)
    {
        const QImage source = detail::supportedImage(input);
        QImage output(source.size(), source.format());
        if (output.isNull())
            throw std::bad_alloc();
        boxBlur(detail::view(source), detail::mutableView(output), kernelWidth, kernelHeight, border);
        detail::finishImage(source, output, border);
        return output;
    }

    inline QImage squareBlur(const QImage &input, int kernelSize, Border border = Border::Reflect101)
    {
        return boxBlur(input, kernelSize, kernelSize, border);
    }
} // namespace mini_blur

#endif // MINI_BLUR_QIMAGE_H
