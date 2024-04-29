#ifndef SIDEBAROPTIONSBUTTON_H
#define SIDEBAROPTIONSBUTTON_H

#include <QObject>
#include <QRadioButton>
#include <QWidget>
#include <QPoint>
#include <QEnterEvent>
#include <QEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QBrush>
#include <QColor>
#include <QLabel>
#include <QSizePolicy>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QRgb>
#include <QLayout>

class SidebarOptionsButton : public QRadioButton
{
    Q_OBJECT
public:
    SidebarOptionsButton(QWidget *parent, int index = 0, int minimumWidth = 40, int fixedHeight = 36);

    void setDisClickedColor(const QColor &color);      // 设置未选中时颜色
    void setClickedColor(const QColor &color);         // 设置选中时颜色
    void setClickedEnterColor(const QColor &color);    // 设置选中时鼠标进入颜色
    void setDisClickedEnterColor(const QColor &color); // 设置未选中时鼠标进入颜色
    void setPromptLineColor(const QColor &color);      // 设置选中时的提示线条颜色
    void setDrawPromptLineEnable(bool enable = true);  // 设置是否绘制选中时的提示线条
    void setFilletRadius(int radius);                  // 设置圆角半径
    int index();                                       // 获取索引值

public slots:
    void setIndex(int index);          // 设置索引
    void setIcon(const QPixmap &icon); // 设置图标
    void setText(const QString &text); // 设置展开后显示的文字

signals:
    void selectedIndex(int index); // 当按钮被选中时,这个信号将会发出索引值

protected:
    bool hitButton(const QPoint &pos) const override;    // 有效范围重设
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    void enterEvent(QEnterEvent *event) override;        // 进入
#else
    void enterEvent(QEvent *event) override;
#endif
    void leaveEvent(QEvent *event) override;             // 离开
    void mousePressEvent(QMouseEvent *event) override;   // 点击
    void mouseReleaseEvent(QMouseEvent *event) override; // 松开
    void paintEvent(QPaintEvent *event) override;

private:
    QColor dis_clicked_Color{qRgb(243, 243, 243)};
    QColor clicked_color{qRgb(233, 233, 233)};
    QColor clicked_enter_color{qRgb(237, 237, 237)};
    QColor dis_clicked_enter_color{qRgb(233, 233, 233)};
    QColor pressColor{qRgb(236, 236, 236)};
    QColor prompt_line_color{qRgb(0, 159, 170)};
    QBrush background_brush{dis_clicked_Color};
    QLabel *icon_label = nullptr;
    QLabel *text_label = nullptr;
    int index_ = 0; // 索引值
    bool is_draw_prompt_line = true;
    int fillet_radius = 4;
};

#endif // SIDEBAROPTIONSBUTTON_H
