#ifndef SIMPLEFRAMELESSWINDOW_H
#define SIMPLEFRAMELESSWINDOW_H

#include <QObject>
#include <QWidget>
#include "framelesswindowbase.h"
#include "precompile_utils/precompile.h"

// This class can only be used on Windows systems
// This class is a window with a title bar without an icon or title
class MaximizeButton;
class SimpleFramelessWindow : public FramelessWindowBase
{
public:
    explicit SimpleFramelessWindow(QWidget *parent = nullptr);
    virtual void deleteTitleBar() override;

protected:
    explicit SimpleFramelessWindow(QWidget *parent, Type type);

    F_NATIVEEVENT;
    F_CHANGEEVENT;
    MaximizeButton *maximize_button;
};

#endif // SIMPLEFRAMELESSWINDOW_H
