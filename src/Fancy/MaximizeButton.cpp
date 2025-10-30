//
// Created by TX on 2025/9/19.
//

#include "MaximizeButton.h"

#include "Defs.hpp"
#include "Palette.h"
#include "SvgWidget.h"

namespace fancy
{
    MaximizeButton::MaximizeButton(QWidget *parent) :
        UniversalTitleBarButton(parent)
    {
        svgWidget()->reload(AntDesignIcons::Square);

        connect(&Palette::palette(), &Palette::appThemeChange, svgWidget(), &SvgWidget::load);
        connect(this, &MaximizeButton::enter, this, &MaximizeButton::onEnter);
        connect(this, &MaximizeButton::leave, this, &MaximizeButton::onLeave);
        connect(this, &MaximizeButton::mousePress, this, &MaximizeButton::onMousePress);
        connect(this, &MaximizeButton::mouseRelease, this, &MaximizeButton::onMouseRelease);
        connect(this, &MaximizeButton::mouseMove, this, &MaximizeButton::onMouseMove);
        connect(this, &MaximizeButton::stateChange, this, &MaximizeButton::loadIcon);
    }


    void MaximizeButton::loadIcon(MaximizeBtnState state) const
    {
        if (state == MaximizeBtnState::Normal)
            svgWidget()->reload(AntDesignIcons::Square);
        else
            svgWidget()->reload(AntDesignIcons::SquareMultiple);
    }

    void MaximizeButton::onEnter()
    {
        setTitleBtnState(TitleBtnState::Hover);
        update();
    }

    void MaximizeButton::onLeave()
    {
        setTitleBtnState(TitleBtnState::Normal);
        update();
    }

    void MaximizeButton::onMousePress()
    {
        setTitleBtnState(TitleBtnState::Pressed);
        update();
    }

    void MaximizeButton::onMouseRelease(bool isInSelf)
    {
        if (isInSelf)
            setTitleBtnState(TitleBtnState::Hover);
        else
            setTitleBtnState(TitleBtnState::Normal);
        update();
    }

    void MaximizeButton::onMouseMove(bool isInSelf)
    {
        if (isInSelf)
            setTitleBtnState(TitleBtnState::Pressed);
        else
            setTitleBtnState(TitleBtnState::PressedOut);
        update();
    }
} // fancy
