//
// Created by TX on 2025/9/15.
//

#include "Icon.h"

namespace fancy
{
    Icon::Icon() :
        _engine(new IconEngine()),
        _icon(std::make_shared<QIcon>(_engine)) {}

    Icon::Icon(const IconId &id) :
        Icon()
    {
        addIconEnum(id);
    }

    Icon::Icon(const QString &fileName) :
        Icon()
    {
        addFile(fileName);
    }

    Icon::Icon(const QPixmap &pixmap) :
        Icon()
    {
        addPixmap(pixmap);
    }

    Icon::Icon(const Icon &other) :
        _engine(new IconEngine(*other._engine)),
        _icon(std::make_shared<QIcon>(_engine)) {}

    Icon::Icon(Icon &&other) noexcept :
        _engine(other._engine),
        _icon(std::move(other._icon))
    {
        other._engine = nullptr;
    }

    Icon &Icon::operator=(const Icon &other)
    {
        if (this != &other)
        {
            _engine = new IconEngine(*other._engine);
            _icon = std::make_shared<QIcon>(_engine);
        }
        return *this;
    }

    Icon &Icon::operator=(Icon &&other) noexcept
    {
        if (this != &other)
        {
            _engine = other._engine;
            _icon = std::move(other._icon);
            other._engine = nullptr;
        }
        return *this;
    }

    Icon::operator const QIcon &() const
    {
        return *_icon;
    }

    Icon::~Icon() = default;

    QPixmap Icon::pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) const
    {
        return _icon->pixmap(size, mode, state);
    }

    QPixmap Icon::pixmap(int w, int h, QIcon::Mode mode, QIcon::State state) const
    {
        return pixmap(QSize(w, h), mode, state);
    }

    QPixmap Icon::pixmap(int extent, QIcon::Mode mode, QIcon::State state) const
    {
        return pixmap(QSize(extent, extent), mode, state);
    }

    QPixmap Icon::pixmap(const QSize &size, qreal devicePixelRatio, QIcon::Mode mode, QIcon::State state) const
    {
        return _icon->pixmap(size, devicePixelRatio, mode, state);
    }

    QSize Icon::actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) const
    {
        return _icon->actualSize(size, mode, state);
    }

    void Icon::addPixmap(const QPixmap &pixmap, QIcon::Mode mode, QIcon::State state)
    {
        _engine->addPixmap(pixmap, mode, state);
    }

    void Icon::addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state)
    {
        _engine->addFile(fileName, size, mode, state);
    }

    void Icon::addIconEnum(const IconId &id, QIcon::Mode mode, QIcon::State state, const QColor &color)
    {
        _engine->addIconEnum(id, mode, state, color);
    }

    void Icon::useAccentColor(bool use) const
    {
        _engine->useAccentColor(use);
    }

    void Icon::useAccentTextColor(bool use) const
    {
        _engine->useAccentTextColors(use);
    }

    void Icon::stateOnUseAccentTextColor(bool use) const
    {
        _engine->stateOnUseAccentTextColor(use);
    }

    void Icon::setCachePolicy(const IconCachePolicy policy)
    {
        IconEngine::setCachePolicy(policy);
    }

    void Icon::clearShardCache()
    {
        IconEngine::clearShardCache();
    }

    bool Icon::isNull() const
    {
        return _engine->isNull();
    }
} // fancy
