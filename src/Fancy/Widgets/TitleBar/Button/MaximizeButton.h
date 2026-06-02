//
// Created by TX on 2025/9/19.
//

#ifndef QWIDGET_FANCYUI_MAXIMIZEBUTTON_H
#define QWIDGET_FANCYUI_MAXIMIZEBUTTON_H
#include "UniversalTitleBarButton.h"

namespace fancy
{
    enum class MaximizeBtnState;
    enum class TitleBtnState;
    class SvgWidget;

    class MaximizeButton : public UniversalTitleBarButton
    {
        Q_OBJECT
        friend class TitleBar;
        friend class Window;

    public:
        explicit MaximizeButton(QWidget *parent);

    private slots:
        void loadIcon(MaximizeBtnState state) const;

        // 最大化按钮无法获取到Qt消息,用以下函数处理
        void onEnter();

        void onLeave();

        void onMousePress();

        void onMouseRelease(bool isInSelf);

        void onMouseMove(bool isInSelf);

    signals:
        void stateChange(MaximizeBtnState state) const;

        // 接受不到Qt消息则在nativeEvent中发射下面这些信号即可处理鼠标消息
        void enter() const;

        void leave() const;

        void mousePress() const;

        void mouseRelease(bool isInSelf) const;

        void mouseMove(bool isInSelf) const;
    };
} // fancy

#endif //QWIDGET_FANCYUI_MAXIMIZEBUTTON_H
