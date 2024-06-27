#ifndef SIMPLETITLEBAR_H
#define SIMPLETITLEBAR_H

#include <QObject>
#include <QWidget>
#include "titlebarbase.h"
#include "minimizebutton.h"
#include "maximizebutton.h"

class SimpleTitleBar : public TitleBarBase
{
    Q_OBJECT
public:
    explicit SimpleTitleBar(QWidget *parent);
    MinimizeButton *minimizeButton() const;
    MaximizeButton *maximizeButton() const;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    MinimizeButton *minimize_button;
    MaximizeButton *maximize_button;
};

#endif // SIMPLETITLEBAR_H
