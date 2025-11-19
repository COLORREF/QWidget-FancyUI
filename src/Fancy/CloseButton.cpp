//
// Created by TX on 2025/9/19.
//

#include "CloseButton.h"

#include <QMouseEvent>

#include "Defs.h"
#include "IconEngine.h"
#include "Palette.h"
#include "SvgWidget.h"

namespace fancy
{
    CloseButton::CloseButton(QWidget *parent) :
        UniversalTitleBarButton(parent)
    {
        svgWidget()->reload(AntDesignIcons::Close);

        connect(this, &CloseButton::clicked, window(), &QWidget::close);
    }

    QBrush CloseButton::maskBrush() const
    {
        switch (auto &pal = Palette::palette(); titleBtnState())
        {
            case TitleBtnState::Normal :
                return pal[ColorRole::MaskNormal];
            case TitleBtnState::Hover :
                return pal[ColorRole::MaskHover];
            case TitleBtnState::Pressed :
                return pal[ColorRole::MaskBrightPressed];
            case TitleBtnState::PressedOut :
                return pal[ColorRole::MaskNormal];
            default :
                return pal[ColorRole::MaskNormal];
        }
    }


    void CloseButton::enterEvent(QEnterEvent *event)
    {
        svgWidget()->cache().first = IconEngine::singleRender(AntDesignIcons::Close, Qt::GlobalColor::white);
        svgWidget()->load();
        setTitleBtnState(TitleBtnState::Hover);
        update();
        QPushButton::enterEvent(event);
    }

    void CloseButton::leaveEvent(QEvent *event)
    {
        svgWidget()->cache().first = IconEngine::iconData(AntDesignIcons::Close);
        svgWidget()->load();
        setTitleBtnState(TitleBtnState::Normal);
        update();
        QPushButton::leaveEvent(event);
    }

    void CloseButton::mouseMoveEvent(QMouseEvent *event)
    {
        // 不开启鼠标追踪只有左键按下才能触发
        if (rect().contains(event->pos()))
        {
            svgWidget()->cache().first = IconEngine::singleRender(AntDesignIcons::Close, Qt::GlobalColor::white);
            setTitleBtnState(TitleBtnState::Pressed);
        }
        else
        {
            svgWidget()->cache().first = IconEngine::iconData(AntDesignIcons::Close);
            setTitleBtnState(TitleBtnState::PressedOut);
        }
        svgWidget()->load();
        update();
        QPushButton::mouseMoveEvent(event);
    }

    void CloseButton::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        if (clearBeforeNewPaint() && (titleBtnState() == TitleBtnState::Normal || titleBtnState() == TitleBtnState::PressedOut))
        {
            painter.setCompositionMode(QPainter::CompositionMode::CompositionMode_Clear);
            painter.eraseRect(rect());
        }
        painter.setPen(Qt::PenStyle::NoPen);
        //背景色
        if (titleBtnState() == TitleBtnState::Hover || titleBtnState() == TitleBtnState::Pressed)
        {
            painter.setBrush(Palette::palette().color(ColorRole::WindowBright));
            painter.drawRoundedRect(rect(), radius(), radius());
        }

        // 遮罩色
        painter.setBrush(maskBrush());
        painter.drawRoundedRect(rect(), radius(), radius());
    }
} // fancy
