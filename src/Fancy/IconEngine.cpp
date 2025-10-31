//
// Created by TX on 2025/9/10.
//

#include "IconEngine.h"

#include <QApplication>

#include "Palette.h"

namespace fancy
{
    IconCachePolicy IconEngine::_cachePolicy = IconCachePolicy::NoCache;
    QHash<IconId, QHash<QColor, QSvgRenderer *> > IconEngine::_shardCache{};

    QString IconEngine::iconPath(const IconId &id)
    {
#define ICON_PATH_FUNC_IMP(Style) if(style_name == #Style){if (const auto mg_em = magic_enum::enum_cast<Style>(id.value); mg_em.has_value())return iconPath<Style>(mg_em.value());}
        const QString style_name(magic_enum::enum_name<IconStyle>(id.style).data());
        ICON_PATH_FUNC_IMP(AntDesignIcons)
        ICON_PATH_FUNC_IMP(BootstrapIcons)
        ICON_PATH_FUNC_IMP(IconPark)
        return {};
    }

    QByteArray IconEngine::iconData(const IconId &id)
    {
        return SvgParsing::loadFromFile(iconPath(id));
    }

    QByteArray IconEngine::singleRender(const IconId &id, const QColor &color)
    {
        if (const QString style_name(magic_enum::enum_name<IconStyle>(id.style).data()); style_name == "AntDesignIcons")
        {
            if (const auto mg_em = magic_enum::enum_cast<AntDesignIcons>(id.value); mg_em.has_value())
                return singleRender<AntDesignIcons>(mg_em.value(), color);
        }
        else if (style_name == "BootstrapIcons")
        {
            if (const auto mg_em = magic_enum::enum_cast<BootstrapIcons>(id.value); mg_em.has_value())
                return singleRender<BootstrapIcons>(mg_em.value(), color);
        }
        else if (style_name == "IconPark")
        {
            if (const auto mg_em = magic_enum::enum_cast<IconPark>(id.value); mg_em.has_value())
                return singleRender<IconPark>(mg_em.value(), color);
        }
        return {};
    }

    QHash<IconId, QSvgRenderer *> IconEngine::cacheAll(const IconStyle style, const QColor &color)
    {
        QHash<IconId, QSvgRenderer *> cache;

        if (const QString style_name(magic_enum::enum_name<IconStyle>(style).data()); style_name == "AntDesignIcons")
        {
            for (const auto em: magic_enum::enum_values<AntDesignIcons>())
            {
                cache[{IconStyle::AntDesignIcons, static_cast<quint64>(em)}] = new QSvgRenderer(singleRender<AntDesignIcons>(em, color), qApp);
                return cache;
            }
        }
        else if (style_name == "BootstrapIcons")
        {
            for (const auto em: magic_enum::enum_values<BootstrapIcons>())
            {
                cache[{IconStyle::BootstrapIcons, static_cast<quint64>(em)}] = new QSvgRenderer(singleRender<BootstrapIcons>(em, color), qApp);
                return cache;
            }
        }
        else if (style_name == "IconPark")
        {
            for (const auto em: magic_enum::enum_values<BootstrapIcons>())
            {
                cache[{IconStyle::IconPark, static_cast<quint64>(em)}] = new QSvgRenderer(singleRender<BootstrapIcons>(em, color), qApp);
                return cache;
            }
        }
        return cache;
    }

    void IconEngine::useAccentColor(const bool use)
    {
        _useAccentColor = use;
        _useAccentTextColor = false;
    }

    void IconEngine::useAccentTextColors(const bool use)
    {
        _useAccentTextColor = use;
        _useAccentColor = false;
    }

    void IconEngine::stateOnUseAccentTextColor(bool use)
    {
        _stateOnUseAccentTextColor = use;
    }

    bool IconEngine::isNull()
    {
        return _files.isEmpty() && _pixmaps.isEmpty() && _iconEnums.isEmpty();
    }

    IconEngine::IconEngine() :
        _useAccentColor(false),
        _useAccentTextColor(false),
        _stateOnUseAccentTextColor(false) {}

    IconEngine::IconEngine(const IconEngine &other) :
        _files(other._files),
        _pixmaps(other._pixmaps),
        _iconEnums(other._iconEnums),
        _useAccentColor(other._useAccentColor),
        _useAccentTextColor(other._useAccentTextColor),
        _stateOnUseAccentTextColor(other._stateOnUseAccentTextColor)
    {
        // 独立缓存为什么要拷贝？？？
        // // 深拷贝 _perCache
        // for (auto it = other._perCache.cbegin(); it != other._perCache.cend(); ++it)
        //     for (auto it2 = it.value().cbegin(); it2 != it.value().cend(); ++it2)
        //         _perCache[it.key()][it2.key()] = {it2.value().first, new QSvgRenderer(it2.value().first)};
    }

    IconEngine::IconEngine(IconEngine &&other) noexcept :
        _files(std::move(other._files)),
        _pixmaps(std::move(other._pixmaps)),
        _iconEnums(std::move(other._iconEnums)),
        _perCache(std::move(other._perCache)),
        _useAccentColor(other._useAccentColor),
        _useAccentTextColor(other._useAccentTextColor),
        _stateOnUseAccentTextColor(other._stateOnUseAccentTextColor)
    {
        other._perCache.clear();
    }

    IconEngine &IconEngine::operator=(const IconEngine &other)
    {
        if (this == &other)
            return *this;
        deletePerCache();
        _perCache.clear();
        _files = other._files;
        _pixmaps = other._pixmaps;
        _iconEnums = other._iconEnums;
        _useAccentColor = other._useAccentColor;
        _useAccentTextColor = other._useAccentTextColor;
        _stateOnUseAccentTextColor = other._stateOnUseAccentTextColor;
        // _perCache 独立缓存，不拷贝
        return *this;
    }

    IconEngine &IconEngine::operator=(IconEngine &&other) noexcept
    {
        if (this == &other)
            return *this;
        deletePerCache(); // 释放已有缓存
        _perCache.clear();
        _files = std::move(other._files);
        _pixmaps = std::move(other._pixmaps);
        _iconEnums = std::move(other._iconEnums);
        _perCache = std::move(other._perCache);
        _useAccentColor = other._useAccentColor;
        _useAccentTextColor = other._useAccentTextColor;
        _stateOnUseAccentTextColor = other._stateOnUseAccentTextColor;
        other._perCache.clear(); // 防止析构重复释放
        return *this;
    }

    IconEngine::~IconEngine()
    {
        deletePerCache();
    }

    QPixmap IconEngine::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state)
    {
        QPixmap pixmap(size);
        pixmap.fill(Qt::GlobalColor::transparent);
        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        this->paint(&painter, QRect{{0, 0}, size}, mode, state);
        return pixmap;
    }

    void IconEngine::paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state)
    {
        static Key keys[]{
            {QIcon::Mode::Normal, QIcon::State::Off},
            {QIcon::Mode::Normal, QIcon::State::On},
            {QIcon::Mode::Disabled, QIcon::State::Off},
            {QIcon::Mode::Disabled, QIcon::State::On},
            {QIcon::Mode::Active, QIcon::State::Off},
            {QIcon::Mode::Active, QIcon::State::On},
            {QIcon::Mode::Selected, QIcon::State::Off},
            {QIcon::Mode::Selected, QIcon::State::On}
        };

        // 根据 ICON_ENUM 绘制，ICON_ENUM 精确查找失败，根据从 addFile 和 addPixmap 添加的内容中进行查找并绘制
        if (const Key key{mode, state}; drawForIconEnum(key, painter, rect) || drawForNative(key, painter, rect))
            return; // 查找成功，绘制后直接返回

        // 均为空，为空图标，不绘制
        if (_iconEnums.isEmpty() && _files.isEmpty() && _pixmaps.isEmpty())
            return;

        // 非空图标但精确查找失败，按 keys 顺序遍历查找，直到找到可绘制内容
        for (const auto &key: keys)
            if (drawForIconEnum(key, painter, rect) || drawForNative(key, painter, rect))
                return;

        // 均未找到，为空图标，不绘制
    }

    QIconEngine *IconEngine::clone() const
    {
        return new IconEngine(*this);
    }

    void IconEngine::addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state)
    {
        const Key key{mode, state};
        // 如果已有 pixmap 或 ICON_ENUM 数据，移除它，保证最后添加的状态有效
        if (_pixmaps.contains(key))
            _pixmaps.remove(key);
        if (_iconEnums.contains(key))
            _iconEnums.remove(key);
        _files[key] = fileName;
    }

    void IconEngine::addPixmap(const QPixmap &pixmap, QIcon::Mode mode, QIcon::State state)
    {
        const Key key{mode, state};
        // 如果已有 fileName 或 ICON_ENUM 数据，移除它，保证最后添加的状态有效
        if (_files.contains(key))
            _files.remove(key);
        if (_iconEnums.contains(key))
            _iconEnums.remove(key);
        _pixmaps[{mode, state}] = pixmap;
    }

    void IconEngine::addIconEnum(const IconId &id, QIcon::Mode mode, QIcon::State state, const QColor &color)
    {
        const Key key{mode, state};
        // 如果已有 fileName 或 pixmap 数据，移除它，保证最后添加的状态有效
        if (_files.contains(key))
            _files.remove(key);
        if (_pixmaps.contains(key))
            _pixmaps.remove(key);
        _iconEnums[key] = {id, color};
    }

    void IconEngine::setCachePolicy(const IconCachePolicy policy)
    {
        if (_cachePolicy == IconCachePolicy::NoCache && policy != _cachePolicy)
            _cachePolicy = policy;
    }

    void IconEngine::clearShardCache()
    {
        for (auto &cache: _shardCache)
            for (const QSvgRenderer *renderer: cache)
                delete renderer;
        _shardCache.clear();
    }

    bool IconEngine::noCachePaint(const IconId &id, QPainter *painter, const QRect &rect, const QColor &color) const
    {
        QSvgRenderer renderer;
        renderer.load(singleRender(id, color));
        renderer.render(painter, rect);
        return true;
    }

    bool IconEngine::perShardCachePaint(const IconId &id, QHash<IconId, QHash<QColor, QSvgRenderer *> > &cache, QPainter *painter, const QRect &rect, const QColor &color)
    {
        QHash<QColor, QSvgRenderer *> &cache_ = cache[id];
        if (QSvgRenderer *renderer = cache_[color])
            renderer->render(painter, rect);
        else
        {
            // 未存储当前颜色
            renderer = new QSvgRenderer(singleRender(id, color), qApp);
            cache_[color] = renderer;
            renderer->render(painter, rect);
        }
        return true;
    }

    bool IconEngine::drawForIconEnum(const Key &key, QPainter *painter, const QRect &rect)
    {
        // 若非 Normal 状态且未存储，则优先绘制 Normal 状态下对应 state 状态的图标
        if (key.first != QIcon::Mode::Normal && !_iconEnums.contains(key))
            return drawForIconEnum({QIcon::Mode::Normal, key.second}, painter, rect);

        if (_iconEnums.contains(key))
        {
            auto [id, col] = _iconEnums[key];
            auto &pal = Palette::palette();

            QColor color;
            if (_useAccentTextColor || (_stateOnUseAccentTextColor && key.second == QIcon::State::On))
                color = pal[ColorRole::AppAccentText];
            else if (_useAccentColor)
                color = pal[ColorRole::AppAccent];
            else if (col.isValid())
                color = col;
            else
                color = pal[ColorRole::Text];


            switch (_cachePolicy)
            {
                case IconCachePolicy::NoCache :
                    return noCachePaint(id, painter, rect, color);
                case IconCachePolicy::PerInstance : // 独立缓存
                    return perShardCachePaint(id, _perCache, painter, rect, color);
                case IconCachePolicy::SharedCache : // 共享缓存
                    return perShardCachePaint(id, _shardCache, painter, rect, color);
            }
        }
        return false;
    }

    bool IconEngine::drawForNative(const Key &key, QPainter *painter, const QRect &rect)
    {
        // 若非 Normal 状态且未存储，绘制 Normal 状态，对应 state 状态
        if (key.first != QIcon::Mode::Normal && !_files.contains(key) && !_pixmaps.contains(key))
            return drawForNative({QIcon::Mode::Normal, key.second}, painter, rect);


        if (_files.contains(key))
        {
            if (const QString &file_name = _files[key]; QFileInfo(file_name).suffix().toLower() == "svg")
            {
                QSvgRenderer renderer(file_name);
                renderer.render(painter, rect);
            }
            else
                painter->drawPixmap(rect, QPixmap{file_name}.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            return true;
        }
        if (_pixmaps.contains(key))
        {
            painter->drawPixmap(rect, _pixmaps[key].scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            return true;
        }
        return false;
    }

    void IconEngine::deletePerCache()
    {
        for (auto &cache: _perCache)
            for (const QSvgRenderer *renderer: cache)
                delete renderer;
    }
} // fancy
