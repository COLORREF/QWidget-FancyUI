#ifndef SYSTEMCOLORWINDOW_H
#define SYSTEMCOLORWINDOW_H

#include <QObject>
#include <QWidget>
#include "transparenteffectwindowbase.h"

class SystemColorWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit SystemColorWindow(QWidget *parent = nullptr);
};

#endif // SYSTEMCOLORWINDOW_H
