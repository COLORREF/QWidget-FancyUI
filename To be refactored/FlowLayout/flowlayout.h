#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QObject>
#include <QWidget>
#include <QList>
#include <QLayoutItem>
#include <QPropertyAnimation>
#include <QRect>
#include <QPoint>
#include <QEasingCurve>
#include <QParallelAnimationGroup>
#include <QSize>
#include <QtMath>

class FlowLayout : public QLayout
{
    Q_OBJECT
public:    
    // parent: parent window or layout
    // needAni: whether to add moving animation
    //isTight: whether to use the tight layout when widgets are hidden
    FlowLayout(QWidget* parent,bool needAni=false,bool isTight = false);

    void addItem(QLayoutItem* item)override;
    void addWidget(QWidget* w);
    void setAnimation(int duration, const QEasingCurve& ease = QEasingCurve::Linear);//set the moving animation
    int count()const override;
    QLayoutItem *itemAt(int index) const override;
    QLayoutItem *takeAt(int index) override;
    QWidget* removeWidget(QWidget* widget);
    void removeAllWidgets();//remove all widgets from layout
    void takeAllWidgets();// remove all widgets from layout and delete them
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth()const override;
    int heightForWidth(int width)const override;//get the minimal height according to width
    void setGeometry(const QRect& rect) override;
    QSize sizeHint()const override;
    QSize minimumSize() const override;
    void setVerticalSpacing(int spacing);//set vertical spacing between widgets
    int verticalSpacing()const;//get vertical spacing between widgets
    void setHorizontalSpacing(int spacing);//set horizontal spacing between widgets
    int horizontalSpacing()const;//get horizontal spacing between widgets

private:
    int doLayout(const QRect& rect, bool move)const;//adjust widgets position according to the window size

private:
    QList<QLayoutItem*> items;
    QList<QPropertyAnimation*>anis;
    QParallelAnimationGroup* aniGroup{new QParallelAnimationGroup(this)};
    int verticalSpacing_ = 10;
    int horizontalSpacing_ = 10;
    int duration=300;//动画运行时间
    QEasingCurve ease{QEasingCurve::Type::Linear};//动画缓动曲线
    bool needAni;
    bool isTight;
};

#endif // FLOWLAYOUT_H
