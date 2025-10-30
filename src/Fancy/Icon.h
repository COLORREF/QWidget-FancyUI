//
// Created by TX on 2025/9/15.
//

#ifndef QWIDGET_FANCYUI_ICON_H
#define QWIDGET_FANCYUI_ICON_H
#include <QIcon>
#include <memory>

#include "ControlState.h"
#include "Defs.hpp"
#include "IconEngine.h"

namespace fancy
{
    class Icon
    {
    public:
        Icon();

        explicit Icon(const IconId &id);

#define ICON_CONSTRUCTOR(Style) explicit Icon(Style em) : Icon(){ addIconEnum(em); }

        ICON_CONSTRUCTOR(AntDesignIcons);

        ICON_CONSTRUCTOR(BootstrapIcons);

        ICON_CONSTRUCTOR(IconPark);

        explicit Icon(const QString &fileName);

        explicit Icon(const QPixmap &pixmap);

        Icon(const Icon &other);

        Icon(Icon &&other) noexcept;

        Icon &operator=(const Icon &other);

        Icon &operator=(Icon &&other) noexcept;

        operator const QIcon &() const;

        explicit Icon(const QIcon &) = delete;

        explicit Icon(QIcon &&) = delete;

        explicit Icon(QIconEngine *) = delete;

        explicit Icon(IconEngine *) = delete;

        ~Icon();

        [[nodiscard]] QPixmap pixmap(const QSize &size, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off) const;

        [[nodiscard]] QPixmap pixmap(int w, int h, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off) const;

        [[nodiscard]] QPixmap pixmap(int extent, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off) const;

        [[nodiscard]] QPixmap pixmap(const QSize &size, qreal devicePixelRatio, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off) const;

        [[nodiscard]] QSize actualSize(const QSize &size, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off) const;

        void addPixmap(const QPixmap &pixmap, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off);

        void addFile(const QString &fileName, const QSize &size = QSize(), QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off);

#define ICON_ADDICONENUM(Style) \
void addIconEnum(Style iconEnum, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off, const QColor &color = QColor{}) \
{ _engine->addIconEnum(iconEnum, mode, state, color);}

        void addIconEnum(const IconId &id, QIcon::Mode mode = QIcon::Mode::Normal, QIcon::State state = QIcon::State::Off, const QColor &color = QColor{});

        ICON_ADDICONENUM(AntDesignIcons);

        ICON_ADDICONENUM(BootstrapIcons);

        ICON_ADDICONENUM(IconPark);

        void useAccentColor(bool use) const;

        void useAccentTextColor(bool use) const;

        void stateOnUseAccentTextColor(bool use) const;

        static void setCachePolicy(IconCachePolicy policy);

        static void clearShardCache();

        [[nodiscard]] bool isNull() const;

    private:
        IconEngine *_engine;
        std::shared_ptr<QIcon> _icon;
    };
} // fancy

#endif //QWIDGET_FANCYUI_ICON_H
