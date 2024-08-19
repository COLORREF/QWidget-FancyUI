#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QWidget>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = nullptr);
    explicit PushButton(const QString &text, QWidget *parent = nullptr);
    PushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
};

#endif // PUSHBUTTON_H
