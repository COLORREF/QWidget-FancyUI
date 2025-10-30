//
// Created by TX on 2025/10/6.
//

#include "UniversalTitleBarButton.h"
#include <QMouseEvent>

#include "Defs.hpp"
#include "IconEngine.h"
#include "Palette.h"
#include "SvgWidget.h"

namespace fancy
{
    UniversalTitleBarButton::UniversalTitleBarButton(QWidget *parent) :
        QPushButton(parent),
        _state(TitleBtnState::Normal),
        _radius(0),
        _clearBeforeNewPaint(false),
        _AutoEnlargeIcon(false),
        _svgWidget(new SvgWidget(this))

    {
        setFixedSize(46, 32);
        int m = width() > height() ? height() : width();
        _svgWidget->resize(m / 2, m / 2);
        _svgWidget->move((width() - _svgWidget->width()) / 2, (height() - _svgWidget->height()) / 2);
        connect(&Palette::palette(), &Palette::appThemeChange, _svgWidget, &SvgWidget::load);
    }

    void UniversalTitleBarButton::setIcon(const IconId &id)
    {
        _svgWidget->reload(id);
    }

    QBrush UniversalTitleBarButton::maskBrush() const
    {
        switch (auto &pal = Palette::palette(); _state)
        {
            case TitleBtnState::Normal :
                return pal[ColorRole::MaskNormal];
            case TitleBtnState::Hover :
                return pal[ColorRole::MaskHover];
            case TitleBtnState::Pressed :
                return pal[ColorRole::MaskPressed];
            case TitleBtnState::PressedOut :
            default :
                return pal[ColorRole::MaskNormal];
        }
    }

    void UniversalTitleBarButton::enterEvent(QEnterEvent *event)
    {
        _state = TitleBtnState::Hover;
        update();
        QPushButton::enterEvent(event);
    }

    void UniversalTitleBarButton::leaveEvent(QEvent *event)
    {
        _state = TitleBtnState::Normal;
        update();
        QPushButton::leaveEvent(event);
    }

    void UniversalTitleBarButton::mouseReleaseEvent(QMouseEvent *event)
    {
        if (rect().contains(event->pos()))
            _state = TitleBtnState::Hover;
        else
            _state = TitleBtnState::Normal;
        update();
        QPushButton::mouseReleaseEvent(event);
    }

    void UniversalTitleBarButton::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
            _state = TitleBtnState::Pressed;
        update();
        QPushButton::mousePressEvent(event);
    }

    void UniversalTitleBarButton::mouseMoveEvent(QMouseEvent *event)
    {
        // 不开启鼠标追踪只有左键按下才能触发
        if (rect().contains(event->pos()))
            _state = TitleBtnState::Pressed;
        else
            _state = TitleBtnState::PressedOut;
        update();
        QPushButton::mouseMoveEvent(event);
    }

    void UniversalTitleBarButton::resizeEvent(QResizeEvent *event)
    {
        QPushButton::resizeEvent(event);
        if (_AutoEnlargeIcon)
        {
            int m = (width() > height() ? height() : width()) / 2;
            _svgWidget->resize(m, m);
        }
        _svgWidget->move((width() - _svgWidget->width()) / 2, (height() - _svgWidget->height()) / 2);
    }

    void UniversalTitleBarButton::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        if (_clearBeforeNewPaint && (_state == TitleBtnState::Normal || _state == TitleBtnState::PressedOut))
        {
            painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Clear);
            painter.eraseRect(rect());
        }
        painter.setPen(Qt::PenStyle::NoPen);
        // 透明遮罩
        painter.setBrush(maskBrush());
        painter.drawRoundedRect(rect(), _radius, _radius);
    }
} // fancy
