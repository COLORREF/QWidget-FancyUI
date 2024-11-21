#ifndef FANCYICON_H
#define FANCYICON_H

#include <QIcon>
#include <QSvgRenderer>
#include <QPainter>
#include <QPixmap>
#include <QSvgWidget>
#include <QLabel>
#include <QMap>
#include <QIconEngine>
#include <magic_enum/magic_enum.hpp>
#include "FancyTheme.h"
#include "FancySvg.h"
#include "FancyStyle.h"
#include "FancyTheme.h"


enum class FancyIcon
{
    Animation,                  //  动画
    Bilibili,                   //  哔哩哔哩
    BlueTooth,                  //  蓝牙
    BookMark,                   //  书签
    CheckBox,                   //  复选框
    Close,                      //  关闭
    Email,                      //  邮件
    Fx,                         //  函数f(x)
    Gear,                       //  齿轮
    Github,                     //  github
    GlobeEarth,                 //  地球仪
    Home,                       //  主页
    Icons,                      //  图标
    Image,                      //  图片
    Label,                      //  标签
    LeftArrow,                  //  左箭头
    Math,                       //  数学
    Maximize,                   //  窗口最大化
    Minimize,                   //  窗户最小化
    Number0,                    //  0
    Number1,                    //  1
    Number2,                    //  2
    Number3,                    //  3
    Number4,                    //  4
    Number5,                    //  5
    Number6,                    //  6
    Number7,                    //  7
    Number8,                    //  8
    Number9,                    //  9
    Number10,                   //  10
    Number11,                   //  11
    Number12,                   //  12
    Number13,                   //  13
    Number14,                   //  14
    Number15,                   //  15
    Number16,                   //  16
    Number17,                   //  17
    Number18,                   //  18
    Number19,                   //  19
    Number20,                   //  20
    Pin,                        //  大头针
    Question,                   //  问号
    Restore,                    //  窗口还原
    Search,                     //  搜索
    SqrtX,                      //  根号x
    SquarePencil,               //  方块铅笔
    SunMoon,                    //  太阳和月亮
    ThreeHorizontalLines,       //  三条水平线
    ThreeHorizontalPoints,      //  三个水平点
    Wifi,                       //  wifi
    Wrench,                     //  扳手
};

QString fancyIconPath(FancyIcon iconEnum, Theme::Type themeType = Theme::Type::DARK);

QString fancyIconName(FancyIcon iconEnum);


class FancyIconEngine : public QIconEngine
{
    friend class Icon;
public:
    FancyIconEngine(FancyIcon iconEnum):
        QIconEngine()
    {
        this->_path.resize(2);
        this->_path[0] = fancyIconPath(iconEnum,Theme::Type::DARK);
        this->_path[1] = fancyIconPath(iconEnum,Theme::Type::LIGHT);
        this->_disabled = FancySvg::replaceSvgBlack(this->_path[0],ControlColors::controlColors()->disEnabled().lighter(130).name());
    }

    // 由QIcon调用
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)override
    {
        QPixmap pixmap(size);
        pixmap.fill(Qt::GlobalColor::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        this->paint(&painter,{},mode,state);
        return pixmap;
    }

    // 由pixmap函数调用
    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)override
    {
        Q_UNUSED(rect)
        QSvgRenderer renderer;
        if(mode == QIcon::Mode::Disabled)
            renderer.load(this->_disabled);
        else if(state == QIcon::State::On || _ColorChangeBaseOnThemeColor)
        {
            if(ControlColors::autoTextColor(ControlColors::controlColors()->theme()) == Qt::GlobalColor::black)
                renderer.load(this->_path[0]);
            else
                renderer.load(this->_path[1]);
        }
        else if(Theme::isLigth())
            renderer.load(this->_path[0]);
        else
            renderer.load(this->_path[1]);
        renderer.render(painter);
        return;
    }

    QIconEngine* clone() const override
    {
        return new FancyIconEngine(*this);
    }

private:
    QStringList _path;
    QByteArray _disabled;
    bool _ColorChangeBaseOnThemeColor = false;
};


class IconBase
{
protected:
    IconBase(FancyIcon iconEnum):
        _engine{new FancyIconEngine(iconEnum)}{}
    FancyIconEngine* _engine;
};

class Icon : private IconBase , public QIcon
{
public:
    explicit Icon(FancyIcon iconEnum):
        IconBase(iconEnum),
        QIcon(this->_engine)
    {
    }

    QStringList path() const
    {
        return this->_engine->_path;
    }

    void setColorChangeBaseOnThemeColor(bool baseOn)const
    {
        this->_engine->_ColorChangeBaseOnThemeColor = baseOn;
    }
};


class FancyIconWidget : public QSvgWidget
{
public:
    FancyIconWidget(QWidget *parent = nullptr):
        QSvgWidget(parent)
    {
        this->_iconLabel = new QLabel(this);
        // this->_iconLabel->setMargin(1);
        this->_iconLabel->setScaledContents(true);
        this->_iconLabel->setGeometry(QRect({0,0},this->size()));
        this->_fancyIconPath.resize(2);
    }

    void load() = delete;

    inline void clear()
    {
        QSvgWidget::load(QByteArray(R"(<svg xmlns="http://www.w3.org/2000/svg" width="1" height="1"></svg>)"));
        this->_iconLabel->clear();
    }

    void cancelAutoReverse()
    {
        disconnect(Theme::themeObject(), &Theme::themeChange, this, &FancyIconWidget::invert);
    }

    // 使用此函数设置图标可以自动切换深浅色
    void loadFancyIcon(FancyIcon iconEnum, bool autoReverse = true)
    {
        this->clear();
        this->_fancyIconPath[0] = fancyIconPath(iconEnum,Theme::Type::DARK);
        this->_fancyIconPath[1] = fancyIconPath(iconEnum,Theme::Type::LIGHT);

        if(Theme::isLigth())
            QSvgWidget::load(this->_fancyIconPath[0]);
        else
            QSvgWidget::load(this->_fancyIconPath[1]);
        if(autoReverse)
        {
            disconnect(Theme::themeObject(), &Theme::themeChange, this, &FancyIconWidget::invert);
            connect(Theme::themeObject(),&Theme::themeChange,this, &FancyIconWidget::invert);
        }
    }

    void loadPicture(const QPicture& picture)
    {
        this->clear();
        this->_iconLabel->setPicture(picture);
    }

    void loadPixmap(const QPixmap& pixmap)
    {
        this->clear();
        this->_iconLabel->setPixmap(pixmap);
    }

    void loadSvg(const QString& path)
    {
        QSvgWidget::load(path);
    }

    void loadSvg(const QByteArray& data)
    {
        QSvgWidget::load(data);
    }

protected:
    virtual void resizeEvent(QResizeEvent* event)override
    {
        this->_iconLabel->setGeometry(QRect({0,0},this->size()));
        return QSvgWidget::resizeEvent(event);
    }

private slots:
    void invert(Theme::Type themeType)
    {
        if(themeType == Theme::Type::DARK)
            QSvgWidget::load(this->_fancyIconPath[1]);
        else
            QSvgWidget::load(this->_fancyIconPath[0]);
    }

private:
    QStringList _fancyIconPath;
    QLabel* _iconLabel;
};

#endif // FANCYICON_H
