#ifndef FANCYRADIOBUTTON_H
#define FANCYRADIOBUTTON_H

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
public slots:
    void enableJumpAnimation(bool isEnable);
private:
    bool _userSetEmptyText = false;
    bool _userSetNullText = false;
};

#endif // FANCYRADIOBUTTON_H
