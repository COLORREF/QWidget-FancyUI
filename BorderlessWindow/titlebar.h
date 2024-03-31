#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QWidget>
#include <QMouseEvent>
#include <QWindow>

class Border;
class MainArea;
class BorderlessWindow;

class TitleBar : public QWidget
{
    Q_OBJECT
    friend MainArea;

public:
    explicit TitleBar(QWidget *parent = nullptr);

private:
    QHBoxLayout *horizontalLayout = nullptr;
    QLabel *title_label = nullptr;
    QSpacerItem *horizontalSpacer = nullptr;
    QPushButton *mini_button = nullptr;
    QPushButton *max_button = nullptr;
    QPushButton *close_button = nullptr;

    bool isMax = false;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private slots:
    void MaximizeButtonClicked();
};

#endif // TITLEBAR_H
