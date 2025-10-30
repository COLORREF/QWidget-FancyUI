//
// Created by TX on 2025/10/6.
//

#ifndef QWIDGET_FANCYUI_SCROLLAREA_H
#define QWIDGET_FANCYUI_SCROLLAREA_H
#include <QScrollArea>

namespace fancy
{
    class ScrollArea : public QScrollArea
    {
        Q_OBJECT

    public:
        explicit ScrollArea(QWidget *parent = nullptr);
    };
} // fancy

#endif //QWIDGET_FANCYUI_SCROLLAREA_H
