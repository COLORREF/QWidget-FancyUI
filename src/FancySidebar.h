#ifndef FANCYSIDEBAR_H
#define FANCYSIDEBAR_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QQueue>
#include <QMap>
#include <QEasingCurve>
#include <QScrollerProperties>
#include "FancyButton.h"
#include "precompile.h"
#include "FancyWidget.h"
#include "FancyIcon.h"


class OptionButtonBase : public TransparentButton
{
    Q_OBJECT
public:
    explicit OptionButtonBase(QWidget *parent, int sidebarWidth = 50);
    explicit OptionButtonBase(const QString& text, QWidget *parent, int sidebarWidth = 50);
    explicit OptionButtonBase(const QString& text, QWidget *parent, FancyIcon iconEnum, int sidebarWidth = 50);
    void setIcon(const QPixmap &icon);
    void setIcon(FancyIcon iconEnum);
    void setIcon(const QPicture& picture);
    void setSvgIcon(const QString& path);
    void setSvgIcon(const QByteArray& data);
    void correctIconCoordinates();//修正图标坐标
    inline void setText(const QString& text){this->_textLabel->setText(text);}
    inline void setDrawIndicator(bool drawIndicator){this->_drawIndicator = drawIndicator;}

protected:
    FancyIconWidget* _iconWidget;
    QLabel* _textLabel;
    QLine _indicator;
    bool _drawIndicator;
    virtual void paintEvent(QPaintEvent *event) override;
};


// 只有一个固定的垂直布局区域，下面有个弹簧，控件不可重叠
class SidebarBase : public QWidget
{
    Q_OBJECT
public:
    explicit SidebarBase(QWidget* parent);
    void addOption(QAbstractButton* option);
protected:
    QVBoxLayout* _verticalLayout;
    QButtonGroup* _btnGroup;
    QSpacerItem* _verticalSpacer;
signals:
    void optionChecked(int id);
};


//上方和下方有一个固定的垂直布局区域，控件不可重叠。中间有一个垂直布局滚动区域,下方有个弹簧
class StandardSidebar : public SidebarBase
{
    Q_OBJECT
public:
    explicit StandardSidebar(QWidget* parent, int upperAreaHeight = 128, int underrAreaHeight = 86);

    F_PROTECTED_POINTER_PUBLIC_GET(QWidget*, upperArea)
    F_PROTECTED_POINTER_PUBLIC_GET(InertiaScrollArea*, scrollArea)
    F_PROTECTED_POINTER_PUBLIC_GET(QWidget*, underrArea)
};


class AnimationOptionButton : public OptionButtonBase
{
    friend class AnimationIndicatorSidebar;
    Q_OBJECT
public:
    explicit AnimationOptionButton(QWidget *parent, int sidebarWidth = 50);
    explicit AnimationOptionButton(const QString& text, QWidget *parent, int sidebarWidth = 50);
    explicit AnimationOptionButton(const QString& text, QWidget *parent, FancyIcon iconEnum, int sidebarWidth = 50);
    F_PROTECTED_PROPERTY(QVariantAnimation*,topToMiddle)
    F_PROTECTED_PROPERTY(QVariantAnimation*,middleToBottom)
    F_PROTECTED_PROPERTY(QLine, upIndicator)
    F_PROTECTED_PROPERTY(QLine, unIndicator)
protected:
    virtual bool eventFilter(QObject *obj, QEvent *event)override;
private:
    inline void topToMiddle()
    {
        this->_topToMiddle->setEasingCurve(QEasingCurve::InQuart);
        this->_topToMiddle->setDirection(QVariantAnimation::Direction::Forward);
        this->_topToMiddle->start();
    }
    inline void middleToTop()
    {
        this->_topToMiddle->setEasingCurve(QEasingCurve::OutQuart);
        this->_topToMiddle->setDirection(QVariantAnimation::Direction::Backward);
        this->_topToMiddle->start();
    }
    inline void middleToBottom()
    {
        this->_middleToBottom->setEasingCurve(QEasingCurve::InQuart);
        this->_middleToBottom->setDirection(QVariantAnimation::Direction::Forward);
        this->_middleToBottom->start();
    }
    inline void bottomToMiddle()
    {
        this->_middleToBottom->setEasingCurve(QEasingCurve::OutQuart);
        this->_middleToBottom->setDirection(QVariantAnimation::Direction::Backward);
        this->_middleToBottom->start();
    }

    F_PAINTEVENT;
};


// 动画指示器侧侧边栏
class AnimationIndicatorSidebar : public StandardSidebar
{
    Q_OBJECT
public:
    explicit AnimationIndicatorSidebar(QWidget* parent, int upperAreaHeight = 128, int underrAreaHeight = 86);
    void addOption(AnimationOptionButton* option);

    F_PROTECTED_PROPERTY(QQueue<int>,checked)
    F_PROTECTED_PROPERTY(QQueue<int>,unChecked)

private slots:
    void startAnimation(int id, bool checked);
};



#endif // FANCYSIDEBAR_H
