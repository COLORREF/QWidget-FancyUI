#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QObject>
#include <QWidget>
#include "titlebarbutton.h"
#include "precompile_utils/precompile.h"

class CloseButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit CloseButton(QWidget *parent);
protected slots:
    void onThemeChange(Theme::Type themeType) override;

    F_ENTEREVENT;
    F_LEAVEEVENT;
    F_MOUSEMOVEEVENT;
};

#endif // CLOSEBUTTON_H
