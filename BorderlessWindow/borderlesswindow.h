#ifndef BORDERLESSWINDOW_H
#define BORDERLESSWINDOW_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>

class Border;
class MainArea;

class BorderlessWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BorderlessWindow(QWidget *parent = nullptr);
    QWidget *qWidgetUseInSetupUi();
    void setWindowTitle(const QString& title);
    void setWindowIcon(const QPixmap& icon);
protected:
    virtual void changeEvent(QEvent *event) override;
private:
    QGridLayout *gridLayout = nullptr;

    Border *left_border = nullptr;
    Border *left_top_border = nullptr;
    Border *right_top_border = nullptr;
    Border *top_border = nullptr;
    Border *bottom_border = nullptr;
    Border *left_bottom_border = nullptr;
    Border *right_border = nullptr;
    Border *right_bottom_border = nullptr;

    MainArea *main_area = nullptr;
};

#endif // BORDERLESSWINDOW_H
