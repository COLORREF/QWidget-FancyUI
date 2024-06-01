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
    void onStateChange(MaximizeButton::State state);

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    virtual void enterEvent(QEnterEvent *event) override;
#else
    virtual void enterEvent(QEvent *event) override;
#endif
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
private:
    State state = State::NORMAL;

signals:
    void stateChange(MaximizeButton::State state);
};

#endif // MAXIMIZEBUTTON_H
