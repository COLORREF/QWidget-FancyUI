#ifndef TITLEBARBASE_H
#define TITLEBARBASE_H

#include <QObject>
#include <QWidget>
#include "closebutton.h"
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWindow>
#include "precompile_utils/precompile.h"


class TitleBarBase : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBarBase(QWidget *parent);

    F_PROTECTED_POINTER_PUBLIC_GET(CloseButton*, closeButton)
    F_PROTECTED_POINTER_PUBLIC_GET(QHBoxLayout *,horizontalLayout)
    F_PROTECTED_POINTER_PUBLIC_GET(QSpacerItem *,horizontalSpacer)
protected:
    F_MOUSEPRESSEVENT;
};

#endif // TITLEBARBASE_H
