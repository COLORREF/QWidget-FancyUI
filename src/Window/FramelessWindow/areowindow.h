#ifndef AREOWINDOW_H
#define AREOWINDOW_H

#include <QObject>
#include <QWidget>
#include "transparenteffectwindowbase.h"
#define ABGR(a, b, g, r) ((COLORREF)(((a & 0xffu) << 24) | ((b & 0xffu) << 16) | ((g & 0xffu) << 8) | (r & 0xffu)))

class AreoWindow : public TransparentEffectWindowBase
{
    Q_OBJECT
public:
    explicit AreoWindow(QWidget *parent = nullptr);
    explicit AreoWindow(COLORREF abgr, QWidget *parent = nullptr);
public slots:
    void changeColor(COLORREF abgr = 0);

private:
    long long window_color = -1;
    void init(long long abgr = -1);
};

#endif // AREOWINDOW_H
