//
// Created by TX on 2025/8/25.
//

#ifndef QWIDGET_FANCYUI_CONTROLSTATE_H
#define QWIDGET_FANCYUI_CONTROLSTATE_H

#include <QStyle>

namespace fancy
{
    enum class ControlType;
    enum class VisualState;

    class ControlState
    {
    public:
        ControlState(QStyle::State state, ControlType type);

        [[nodiscard]] VisualState visualState() const;

        bool operator==(const VisualState state) const { return _state == state; }

        operator VisualState() const { return _state; }

        ControlState &operator=(VisualState state)
        {
            _state = state;
            return *this;
        }

        VisualState _state;
        ControlType _type;
        bool _enable; // 启用
        bool _on; // 选中
        bool _off; // 未选中
        bool _over; // 悬浮
        bool _sunken; // 按下
    };
} // fancy

#endif //QWIDGET_FANCYUI_CONTROLSTATE_H
