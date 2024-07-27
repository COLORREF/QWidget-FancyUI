#ifndef THEMECOLORMANAGEMENT_H
#define THEMECOLORMANAGEMENT_H

#include <QObject>
#include <QVariantAnimation>
#include <QWidget>
#include "theme.h"
#include "precompile.h"

class ThemeColorManagement : public QVariantAnimation
{
    Q_OBJECT
public:
    explicit ThemeColorManagement(QObject *parent = nullptr, const QColor &light = QColor(240, 243, 249), const QColor &dark = QColor(26,32,52));
    void setThemeColor(const QColor &light,const QColor &dark);
    QColor lightColor();
    QColor darkColor();
    bool isEnable();

    F_PUBLIC_PORINTER_GET(QColor,runTimeColor)
    F_PROTECTED_PROPERTY(QColor,runTimeColor)
    F_PRIVATE_PROPERTY_PUBLIC_GETSET(bool,enabled,Enable,)
private slots:
    void updateRunTimeColor(const QVariant &value);
    void onThemeChange(Theme::Type type);
};

#endif // THEMECOLORMANAGEMENT_H
