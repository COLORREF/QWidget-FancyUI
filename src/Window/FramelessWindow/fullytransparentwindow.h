#ifndef FULLYTRANSPARENTWINDOW_H
#define FULLYTRANSPARENTWINDOW_H

#include <QObject>
#include <QWidget>
#include "transparenteffectwindowbase.h"

class FullyTransparentWindow : public TransparentEffectWindowBase
{
public:
    explicit FullyTransparentWindow(QWidget *parent = nullptr);
};

#endif // FULLYTRANSPARENTWINDOW_H
