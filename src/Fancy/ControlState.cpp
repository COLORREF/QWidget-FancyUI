//
// Created by TX on 2025/8/25.
//

#include "ControlState.h"
#include "Defs.hpp"

namespace fancy
{
    ControlState::ControlState(QStyle::State state, ControlType type) :
        _state(VisualState::UnKnown)
    {
        _enable = state & QStyle::StateFlag::State_Enabled;
        _on = state & QStyle::StateFlag::State_On;
        _off = state & QStyle::StateFlag::State_Off;
        _over = state & QStyle::StateFlag::State_MouseOver;
        _sunken = state & QStyle::StateFlag::State_Sunken;
        _type = type;

        _state = visualState();
    }

    VisualState ControlState::visualState() const
    {
        switch (_type)
        {
            case ControlType::IndicatorCheckable :
            {
                if (!_enable)
                    return _on ? VisualState::SelectedDisabled : VisualState::Disabled;
                if (_on)
                    return _over ? (_sunken ? VisualState::SelectedPressed : VisualState::SelectedHover) : VisualState::Selected;
                if (_off)
                    return _over ? (_sunken ? VisualState::Pressed : VisualState::Hover) : VisualState::Normal;
            }
            case ControlType::Widget :
            {
                if (!_enable)
                    return VisualState::Disabled;
                if (_over)
                    return _sunken ? VisualState::Pressed : VisualState::Hover;
                return VisualState::Normal;
            }
            case ControlType::WidgetCheckable :
            {
                if (!_enable)
                    return _on ? VisualState::SelectedDisabled : VisualState::Disabled;
                if (_on)
                    return _over ? (_sunken ? VisualState::SelectedPressed : VisualState::SelectedHover) : VisualState::Selected;
                return _over ? (_sunken ? VisualState::Pressed : VisualState::Hover) : VisualState::Normal;
            }
            default :
                return VisualState::UnKnown;
        }
    }
} // fancy
