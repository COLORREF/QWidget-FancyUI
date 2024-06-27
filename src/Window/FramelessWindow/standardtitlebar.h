#ifndef STANDARDTITLEBAR_H
#define STANDARDTITLEBAR_H

#include <QObject>
#include <QWidget>
#include "simpletitlebar.h"
#include <QLabel>
#include <QStyle>
#include <QFont>
#include <QApplication>
#include <QPalette>
#include <QPainter>
#include "precompile_utils/precompile.h"

class TitleTextLabel : public QLabel
{
    Q_OBJECT
public:
    using QLabel::QLabel;
    F_PAINTEVENT;
    F_PROTECTED_PROPERTY_PUBLIC_GETSET(bool,isClearBeforeNewPaint,IsClearBeforeNewPaint,)// 是否在新的绘制前清空旧的区域
};

class StandardTitleBar : public SimpleTitleBar
{
    Q_OBJECT
private:
    QColor _lightTitltTextColor{Qt::GlobalColor::black};
    QColor _darkTitltTextColor{Qt::GlobalColor::white};
    friend class TitleTextLabel;
public:
    explicit StandardTitleBar(QWidget *parent);
public slots:
    void setIcon(const QPixmap &icon);
    void setTitle(const QString &title);
    void setTitleFont(const QFont &font);
    void setTitleColor(const QColor &globalColor);
    void setTitleColor(const QColor &lightColor, const QColor &darkColor);

protected:
    virtual void onThemeChange(Theme::Type themeType);

    F_PROTECTED_POINTER_PUBLIC_GET(QLabel*,iconLabel)
    F_PROTECTED_POINTER_PUBLIC_GET(TitleTextLabel *,titleLabel)
};

#endif // STANDARDTITLEBAR_H
