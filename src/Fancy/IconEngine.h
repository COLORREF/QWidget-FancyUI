//
// Created by TX on 2025/9/6.
//

#ifndef QWIDGET_FANCYUI_ICONENGINE_H
#define QWIDGET_FANCYUI_ICONENGINE_H

#include <qapplication.h>
#include <QColor>
#include <QFileInfo>
#include <QIconEngine>
#include <QMap>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <QSvgWidget>
#include "Defs.hpp"
#include "IconEnums.h"
#include "SvgParsing.hpp"
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 3000
#include <magic_enum.hpp>




namespace fancy
{
    enum class IconCachePolicy;
    enum class IconStyle : unsigned int;
    struct IconId;

    class IconEngine : public QIconEngine
    {
    public:
        template<class ICON_ENUM>
        static QString iconPath(ICON_ENUM em)
        {
            const char *enum_name = magic_enum::enum_name<ICON_ENUM>(em).data();
            const char *enum_class_name = magic_enum::enum_type_name<ICON_ENUM>().data();
            return QString{":/icons/%1/%2.svg"}.arg(enum_class_name, enum_name);
        }

        template<class ICON_ENUM>
        static QByteArray iconData(ICON_ENUM em)
        {
            return SvgParsing::loadFromFile(iconPath(em));
        }

        template<class ICON_ENUM>
        static QByteArray singleRender(ICON_ENUM em, const QColor &color)
        {
            QByteArray data = iconData(em);
            if constexpr (std::is_same_v<ICON_ENUM, AntDesignIcons>)
            {
                if (color == Qt::GlobalColor::black)
                    return data;
                QMap<QString, QString> changes;
                changes["fill"] = color.name();
                changes["fill-opacity"] = QString::number(color.alphaF());
                data = SvgParsing::replaceAttributesValues(data, changes, {"path"}, true);
            }
            else if constexpr (std::is_same_v<ICON_ENUM, BootstrapIcons>)
            {
                if (color == Qt::GlobalColor::black)
                    return data;
                QMap<QString, QString> changes;
                changes["fill"] = color.name();
                changes["fill-opacity"] = QString::number(color.alphaF());
                data = SvgParsing::replaceAttributesValues(data, changes, {"svg"}, false);
            }
            else if constexpr (std::is_same_v<ICON_ENUM, IconPark>)
            {
                QMap<QString, QString> changes;
                changes["fill"] = color.name();
                changes["stroke"] = color.name();
                changes["fill-opacity"] = QString::number(color.alphaF());
                changes["stroke-opacity"] = QString::number(color.alphaF());
                data = SvgParsing::replaceAttributesValues(data, changes, {"path", "circle", "g", "rect", "ellipse"}, false);
            }
            else
            {
                QMap<QString, QString> changes;
                changes["fill"] = color.name();
                changes["color"] = color.name();
                changes["stroke"] = color.name();
                changes["fill-opacity"] = QString::number(color.alphaF());
                changes["stroke-opacity"] = QString::number(color.alphaF());
                const QStringList tags{"svg", "g", "rect", "circle", "ellipse", "line", "polyline", "polygon", "path", "text", "tspan", "radialGradient", "pattern"};
                data = SvgParsing::replaceAttributesValues(data, changes, tags, false);
            }
            return data;
        }

        template<class ICON_ENUM>
        static QHash<ICON_ENUM, QSvgRenderer *> cacheAll(const QColor &color)
        {
            QHash<ICON_ENUM, QSvgRenderer *> cache;
            for (const auto em: magic_enum::enum_values<ICON_ENUM>())
                cache[em] = new QSvgRenderer(singleRender(em, color), qApp);
            return cache;
        }

        static QString iconPath(const IconId &id);

        static QByteArray iconData(const IconId &id);

        static QByteArray singleRender(const IconId &id, const QColor &color);

        static QHash<IconId, QSvgRenderer *> cacheAll(IconStyle style, const QColor &color);

        void useAccentColor(bool use);

        void useAccentTextColors(bool use);

        void stateOnUseAccentTextColor(bool use);

        bool isNull() override;

        IconEngine();

        IconEngine(const IconEngine &other);

        IconEngine(IconEngine &&other) noexcept;

        IconEngine &operator=(const IconEngine &other);

        IconEngine &operator=(IconEngine &&other) noexcept;

        ~IconEngine() override;

        QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

        void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;

        [[nodiscard]] QIconEngine *clone() const override;

        void addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state) override;

        void addPixmap(const QPixmap &pixmap, QIcon::Mode mode, QIcon::State state) override;

        void addIconEnum(const IconId &id, QIcon::Mode mode, QIcon::State state, const QColor &color = QColor{});

#define ENGINE_ADDICONENUM(Style) \
void addIconEnum(Style iconEnum, QIcon::Mode mode, QIcon::State state, const QColor &color = QColor{}) { addIconEnum(iconId(iconEnum), mode, state, color);}

        ENGINE_ADDICONENUM(AntDesignIcons);

        ENGINE_ADDICONENUM(BootstrapIcons);

        ENGINE_ADDICONENUM(IconPark);

        static void setCachePolicy(IconCachePolicy policy);

        static void clearShardCache();

    private:
        using Key = QPair<QIcon::Mode, QIcon::State>;
        QMap<Key, QString> _files;
        QMap<Key, QPixmap> _pixmaps;
        QMap<Key, QPair<IconId, QColor> > _iconEnums;
        QHash<IconId, QHash<QColor, QSvgRenderer *> > _perCache; // 独立缓存
        bool _useAccentColor;
        bool _useAccentTextColor;
        bool _stateOnUseAccentTextColor;
        static IconCachePolicy _cachePolicy; // 图标缓存策略
        static QHash<IconId, QHash<QColor, QSvgRenderer *> > _shardCache; // 共享缓存

        bool noCachePaint(const IconId &id, QPainter *painter, const QRect &rect, const QColor &color) const;

        bool perShardCachePaint(const IconId &id, QHash<IconId, QHash<QColor, QSvgRenderer *> > &cache, QPainter *painter, const QRect &rect, const QColor &color);

        bool drawForIconEnum(const Key &key, QPainter *painter, const QRect &rect);

        bool drawForNative(const Key &key, QPainter *painter, const QRect &rect);

        void deletePerCache();
    };
} // fancy
#endif //QWIDGET_FANCYUI_ICONENGINE_H
