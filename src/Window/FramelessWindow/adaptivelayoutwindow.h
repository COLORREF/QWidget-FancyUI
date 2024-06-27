#ifndef ADAPTIVELAYOUTWINDOW_H
#define ADAPTIVELAYOUTWINDOW_H

#include <QObject>
#include <QWidget>
#include <QVariantAnimation>
#include <QStackedWidget>
#include "mswindow.h"
#include "precompile_utils/precompile.h"
#include "FramelessWindow/transparenteffectwindowbase.h"



//Code writing
class AnimationStackedWidget : public QStackedWidget
{
public:
    using QStackedWidget::QStackedWidget;
};
//


class AdaptiveLayoutWindow : public MSWindow
{
    Q_OBJECT
public:
    explicit AdaptiveLayoutWindow(QWidget *parent = nullptr, WindowEffectType effectType = WindowEffectType::MICA, bool stackWindow = true);

    F_RESIZEEVENT;
    F_PRIVATE_PROPERTY(QVariantAnimation*,animation)
    F_PRIVATE_PROPERTY(bool,isExpand)

private slots:
    void checkWidth();
};

#endif // ADAPTIVELAYOUTWINDOW_H
