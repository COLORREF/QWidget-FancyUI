//
// Created by TX on 2025/10/23.
//

#ifndef QWIDGET_FANCYUI_SCROLLBAR_H
#define QWIDGET_FANCYUI_SCROLLBAR_H
#include <QScrollBar>

namespace fancy
{
    class ScrollBar : public QScrollBar
    {
        Q_OBJECT

    public:
        explicit ScrollBar(QWidget *parent = nullptr);

        explicit ScrollBar(Qt::Orientation, QWidget *parent = nullptr);
    };
} // fancy

#endif //QWIDGET_FANCYUI_SCROLLBAR_H
