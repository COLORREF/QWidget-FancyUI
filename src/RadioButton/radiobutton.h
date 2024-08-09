#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include <QObject>
#include <QRadioButton>
#include <QWidget>

class RadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit RadioButton(QWidget *parent = nullptr);
    explicit RadioButton(const QString &text, QWidget *parent = nullptr);
};

#endif // RADIOBUTTON_H
