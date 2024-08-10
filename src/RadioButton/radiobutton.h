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
    void setText(const QString &text);
    QString text() const;
private:
    bool _userSetEmptyText = false;
    bool _userSetNullText = false;
};

#endif // RADIOBUTTON_H
