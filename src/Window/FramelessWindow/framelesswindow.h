#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QObject>
#include <QWidget>
#include "simpleframelesswindow.h"

// This class can only be used on Windows systems
// This class is a standard window
class FramelessWindow : public SimpleFramelessWindow
{
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *parent = nullptr);
public slots:
    void setWindowIcon(const QPixmap &icon);
    void setWindowTitle(const QString &title);
    void setTitleFont(const QFont &font);
    void setTitleColor(const QColor &globalColor);
    void setTitleColor(const QColor &lightColor, const QColor &darkColor);
};

#endif // FRAMELESSWINDOW_H
