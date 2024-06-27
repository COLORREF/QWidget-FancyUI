#ifndef ACRYLICEFFECTWINDOW_H
#define ACRYLICEFFECTWINDOW_H

#include <QObject>
#include <QWidget>
#include "transparenteffectwindowbase.h"

class AcrylicEffectWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit AcrylicEffectWindow(QWidget *paernt = nullptr);
};

#endif // ACRYLICEFFECTWINDOW_H
