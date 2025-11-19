//
// Created by TX on 2025/9/17.
//

#ifndef QWIDGET_FANCYUI_SVGWIDGET_H
#define QWIDGET_FANCYUI_SVGWIDGET_H

#include <QSvgWidget>

#include "Defs.h"
#include "IconEnums.h"
#include "SvgParsing.h"

namespace fancy
{
    class SvgWidget : public QSvgWidget
    {
    public:
        explicit SvgWidget(QWidget *parent = nullptr);

        explicit SvgWidget(const QString &file, QWidget *parent = nullptr);

        explicit SvgWidget(const IconId &id, QWidget *parent = nullptr);

#define SVGWIDGET(Style) explicit SvgWidget(Style em, QWidget *parent = nullptr) : SvgWidget(iconId(em), parent) {}

        SVGWIDGET(AntDesignIcons)
        SVGWIDGET(BootstrapIcons)
        SVGWIDGET(IconPark)

        void clear();

        QPair<QByteArray, QByteArray> &cache();

        void reload(const IconId &id);
#define SVGWIDGET_RELOAD(Style) void reload(Style em){ reload(iconId(em)); }
        SVGWIDGET_RELOAD(AntDesignIcons)
        SVGWIDGET_RELOAD(BootstrapIcons)
        SVGWIDGET_RELOAD(IconPark)

    private:
        QPair<QByteArray, QByteArray> _cache;

    public slots:
        void load();
    };
} // fancy

#endif //QWIDGET_FANCYUI_SVGWIDGET_H
