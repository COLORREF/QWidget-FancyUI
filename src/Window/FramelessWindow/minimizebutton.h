#ifndef MINIMIZEBUTTON_H
#define MINIMIZEBUTTON_H

#include <QObject>
#include <QWidget>
#include "titlebarbutton.h"

class MinimizeButton : public TitleBarButton
{
    Q_OBJECT
public:
    explicit MinimizeButton(QWidget *parent);

protected slots:
    void onThemeChange(Theme::Type themeType) override;
};

#endif // MINIMIZEBUTTON_H
