#ifndef SIMPLETITLEBAR_H
#define SIMPLETITLEBAR_H

#include <QObject>
#include <QWidget>
#include "titlebarbase.h"
#include "minimizebutton.h"
#include "maximizebutton.h"
#include "precompile_utils/precompile.h"

class SimpleTitleBar : public TitleBarBase
{
    Q_OBJECT
public:
    explicit SimpleTitleBar(QWidget *parent);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    F_PROTECTED_POINTER_PUBLIC_GET(MinimizeButton*, minimizeButton);
    F_PROTECTED_POINTER_PUBLIC_GET(MaximizeButton*, maximizeButton);
};

#endif // SIMPLETITLEBAR_H
