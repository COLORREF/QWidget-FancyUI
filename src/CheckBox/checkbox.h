#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
#include <QObject>
#include <QWidget>

class CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CheckBox(QWidget *parent = nullptr);
    explicit CheckBox(const QString &text, QWidget *parent = nullptr);
    void setText(const QString &text);
    QString text() const;
private:
    bool _userSetEmptyText = false;
    bool _userSetNullText = false;
};

#endif // CHECKBOX_H
