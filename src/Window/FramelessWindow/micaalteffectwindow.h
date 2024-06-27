#ifndef MICAALTEFFECTWINDOW_H
#define MICAALTEFFECTWINDOW_H

#include <QObject>
#include <QWidget>
#include "transparenteffectwindowbase.h"

class MicaAltEffectWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit MicaAltEffectWindow(QWidget *parent = nullptr);
};

#endif // MICAALTEFFECTWINDOW_H
