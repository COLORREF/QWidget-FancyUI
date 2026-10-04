//
// Created by TX on 2025/8/25.
//

#include "ControlState.h"
#include "Core/Defs.h"

namespace fancy
{
    ControlState::ControlState(QStyle::State state, ControlType type) :
        _state(VisualState::UnKnown)
    {
        _enable = state.testFlag(QStyle::StateFlag::State_Enabled);
        _on = state.testFlag(QStyle::StateFlag::State_On);
        _off = state.testFlag(QStyle::StateFlag::State_Off);
        _partial = state.testFlag(QStyle::StateFlag::State_NoChange);
        _over = state.testFlag(QStyle::StateFlag::State_MouseOver);
        _sunken = state.testFlag(QStyle::StateFlag::State_Sunken);
        _focus = state.testFlag(QStyle::StateFlag::State_HasFocus);
        _type = type;

        _state = visualState();
    }

    VisualState ControlState::visualState() const
    {
        switch (_type)
        {
            case ControlType::IndicatorCheckable :
            case ControlType::WidgetCheckable :
            {
                const bool selected = _on || _partial;
                if (!_enable)
                    return selected ? VisualState::SelectedDisabled : VisualState::Disabled;
                if (selected)
                    return _over ? (_sunken ? VisualState::SelectedPressed : VisualState::SelectedHover) : VisualState::Selected;
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
            case ControlType::TextEdit :
            {
                if (!_enable)
                    return VisualState::Disabled;
                if (_focus)
                    return _over ? VisualState::FocusHover : VisualState::Focus;
                if (_over)
                    return VisualState::Hover;
                return VisualState::Normal;
            }
            default :
                return VisualState::UnKnown;
        }
    }
} // fancy
