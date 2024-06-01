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

class StandardTitleBar : public SimpleTitleBar
{
    Q_OBJECT
private:
    QColor titlt_text_color{Qt::GlobalColor::black};
    friend class TitleTextLabel;
    class TitleTextLabel : public QLabel
    {
    public:
        using QLabel::QLabel;
        void setIsClearBeforeNewPaint(bool isClear = false);

    protected:
        void paintEvent(QPaintEvent *) override;
        bool is_clear_before_new_paint = false; // 是否在新的绘制前清空旧的区域
    };
public:
    explicit StandardTitleBar(QWidget *parent);
    QWidget *iconLabel() const;
    TitleTextLabel *titleLabel() const;
public slots:
    void setIcon(const QPixmap &icon);
    void setTitle(const QString &title);
    void setTitleFont(const QFont &font);
    void setTitleColor(const QColor &titleColor);

protected:
    QLabel *icon_label;
    TitleTextLabel *title_label;
};

#endif // STANDARDTITLEBAR_H
