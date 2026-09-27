#ifndef MINI_BLUR_H
#define MINI_BLUR_H

#include <cstddef>
#include <cstdint>
#include <memory>

namespace mini_blur
{
    // Pixels are interleaved 8-bit channels. stride is the byte distance between rows.
    struct ImageView
    {
        const std::uint8_t *data;
        int width;
        int height;
        int channels; // 1, 2, 3, or 4
        std::size_t stride;
    };

    struct MutableImageView
    {
        std::uint8_t *data;
        int width;
        int height;
        int channels;
        std::size_t stride;
    };

    enum class Border { Reflect101, Reflect, Replicate, ConstantZero };

    // Reuse one context per independent stream. A context is NOT reentrant.
    // threads=0 selects up to four threads; threads=1 disables worker threads.
    // Workers and scratch buffers persist until destruction. Input/output may alias
    // exactly. Kernels are limited to 4095 per axis. All views are isolated ROIs.
    class Context
    {
    public:
        explicit Context(unsigned threads = 0);

        ~Context();

        Context(const Context &) = delete;

        Context &operator=(const Context &) = delete;

        void gaussianBlur(
            ImageView src,
            MutableImageView dst,
            int kernelWidth,
            int kernelHeight,
            double sigmaX = 0,
            double sigmaY = 0,
            Border border = Border::Reflect101);

        void boxBlur(
            ImageView src,
            MutableImageView dst,
            int kernelWidth,
            int kernelHeight,
            Border border = Border::Reflect101);

        [[nodiscard]] unsigned threadCount() const;

        static const char *backend();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

    // Throws std::invalid_argument for invalid geometry or parameters.
    // Uses a reusable, single-thread context local to the calling thread.
    // For high-frequency parallel processing, explicitly reuse Context.
    // src and dst may be the same image. Partial overlap is unsupported.
    void gaussianBlur(
        ImageView src,
        MutableImageView dst,
        int kernelWidth,
        int kernelHeight,
        double sigmaX = 0,
        double sigmaY = 0,
        Border border = Border::Reflect101);

    // Uniform normalized box filter, equivalent in meaning to OpenCV blur().
    // Kernel sizes can be even; the anchor is floor(size / 2), as in OpenCV.
    void boxBlur(
        ImageView src,
        MutableImageView dst,
        int kernelWidth,
        int kernelHeight,
        Border border = Border::Reflect101);

    inline void squareBlur(
        ImageView src,
        MutableImageView dst,
        int kernelSize,
        Border border = Border::Reflect101)
    {
        boxBlur(src, dst, kernelSize, kernelSize, border);
    }
} // namespace mini_blur

#endif // MINI_BLUR_H
