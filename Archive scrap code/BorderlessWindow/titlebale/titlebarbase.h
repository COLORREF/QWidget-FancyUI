#ifndef TITLEBARBASE_H
#define TITLEBARBASE_H

#include <QObject>
#include <QWidget>
#include "closebutton.h"
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWindow>

class TitleBarBase : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBarBase(QWidget *parent);
    QWidget *closeButtontn() const;
    QHBoxLayout *layout() const;

protected:
    CloseButton *close_button;
    QHBoxLayout *horizontal_layout;
    QSpacerItem *horizontal_spacer;
    virtual void mousePressEvent(QMouseEvent *) override;
};

#endif // TITLEBARBASE_H
