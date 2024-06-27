#ifndef MAXIMIZEBUTTON_H
#define MAXIMIZEBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QWidget>
#include "titlebarbutton.h"

class MaximizeButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit MaximizeButton(QWidget *parent);
    enum class State
    {
        MAXIMIZE,
        NORMAL
    };
    MaximizeButton::State maxmizeState();

protected slots:
    void onThemeChange(Theme::Type themeType) override;
    void onStateChange(MaximizeButton::State state);

    // 最大化按钮无法获取到Qt消息,用以下函数处理
    void onEnter();
    void onLeave();
    void onMousePress();
    void onMouseRelease(bool isInSelf);
    void onMouseMove(bool isInSelf);

private:
    State state = State::NORMAL;

signals:
    void stateChange(MaximizeButton::State state);

    // 在nativeEvent中发射下面这些信号即可处理鼠标消息
    void enter();
    void leave();
    void mousePress();
    void mouseRelease(bool isInSelf);
    void mouseMove(bool isInSelf);
};

#endif // MAXIMIZEBUTTON_H
