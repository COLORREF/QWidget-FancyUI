//
// Created by TX on 2025/10/30.
//

#include "BlurCard.h"

#include <QPainterPath>
#include <QPainter>

#include "Core/Defs.h"
#include "utils/General.h"
#include "mini_blur_qimage.h"

namespace fancy
{
    BlurCard::BlurCard(QWidget *parent) :
        BlurCard(parent, nullptr) {}

    BlurCard::BlurCard(QWidget *parent, QWidget *blurredObj) :
        QPushButton(parent),
        _blurRadius(10),
        _radius(10),
        _blurredObj(blurredObj),
        _resolutionScale(1.0) {}

    void BlurCard::setResolutionScale(qreal scale)
    {
        if (qIsFinite(scale) && scale > 0)
            _resolutionScale = scale;
    }

    void BlurCard::blur()
    {
        if (_blurredObj)
        {
            const QImage image = renderWidgetRegion(_blurredObj, getWidgetRectInAncestor(this, _blurredObj), _resolutionScale);
            const int pixelRadius = _blurRadius > 0
                                        ? qMax(1, qRound(qMin(static_cast<qreal>(_blurRadius) * image.devicePixelRatio(), 2047.0)))
                                        : 0;
            if (image.isNull() || pixelRadius == 0)
                setPixmap(QPixmap::fromImage(image));
            else
            {
                // 同一调用线程内的卡片复用一个 Context；首次使用时创建 3 个工作线程。
                static thread_local mini_blur::Context blurContext(4);
                const int kernelSize = pixelRadius * 2 + 1;
                constexpr auto border = mini_blur::Border::Replicate;
                mini_blur::boxBlurTo(blurContext, image, _blurStage1, kernelSize, kernelSize, border);
                mini_blur::boxBlurTo(blurContext, _blurStage1, _blurStage2, kernelSize, kernelSize, border);
                mini_blur::boxBlurTo(blurContext, _blurStage2, _blurOutput, kernelSize, kernelSize, border);
                setPixmap(QPixmap::fromImage(_blurOutput));
            }
            update();
        }
    }

    void BlurCard::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::SmoothPixmapTransform);
        QPainterPath path;
        path.addRoundedRect(rect(), _radius, _radius);
        painter.setClipPath(path);
        if (!_pixmap.isNull())
            painter.drawPixmap(rect(), _pixmap);
    }
} // fancy
