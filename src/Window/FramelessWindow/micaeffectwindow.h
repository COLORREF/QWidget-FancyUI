#ifndef MICAEFFECTWINDOW_H
#define MICAEFFECTWINDOW_H

#include <QObject>
#include <QWidget>
#include <QResizeEvent>
#include "transparenteffectwindowbase.h"

class MicaEffectWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit MicaEffectWindow(QWidget *parent = nullptr);
};

#endif // MICAEFFECTWINDOW_H
