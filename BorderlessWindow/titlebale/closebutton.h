#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPainterPath>
#include "titlebarbutton.h"

class CloseButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit CloseButton(QWidget *parent);
protected slots:
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    virtual void enterEvent(QEnterEvent *event) override;
#else
    virtual void enterEvent(QEvent *event) override;
#endif
    void leaveEvent(QEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *)override;
};

#endif // CLOSEBUTTON_H
