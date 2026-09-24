//
// Created by A1840 on 2026/8/31.
//

#ifndef QWIDGET_FANCYUI_HASH_H
#define QWIDGET_FANCYUI_HASH_H
#include <QHashFunctions>
#include <type_traits>

#include "Core/Defs.h"

namespace fancy
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    inline size_t qHash(const IconId &id, size_t seed = 0) { return ::qHash(id.style, seed) ^ ::qHash(id.value); }
#else

    // 若升级到 C++20，可使用约束模板统一下面的枚举 qHash 重载：
    // template<typename Enum> requires std::is_enum_v<Enum>
    // inline uint qHash(const Enum &e, uint seed = 0) noexcept
    // {
    //     return ::qHash(static_cast<std::underlying_type_t<Enum>>(e), seed);
    // }

    inline uint qHash(Theme e, uint seed = 0) noexcept
    {
        return ::qHash(static_cast<std::underlying_type_t<Theme>>(e), seed);
    }

    inline uint qHash(ColorGroups e, uint seed = 0) noexcept
    {
        return ::qHash(static_cast<std::underlying_type_t<ColorGroups>>(e), seed);
    }

    inline uint qHash(ColorRole e, uint seed = 0) noexcept
    {
        return ::qHash(static_cast<std::underlying_type_t<ColorRole>>(e), seed);
    }

    inline uint qHash(AntDesignIcons e, uint seed = 0) noexcept
    {
        return ::qHash(static_cast<std::underlying_type_t<AntDesignIcons>>(e), seed);
    }

    inline uint qHash(BootstrapIcons e, uint seed = 0) noexcept
    {
        return ::qHash(static_cast<std::underlying_type_t<BootstrapIcons>>(e), seed);
    }

    inline uint qHash(IconPark e, uint seed = 0) noexcept
    {
        return ::qHash(static_cast<std::underlying_type_t<IconPark>>(e), seed);
    }

    inline uint qHash(const IconId &id, uint seed = 0) noexcept
    {
        return ::qHash(id.value, seed) ^ ::qHash(static_cast<std::underlying_type_t<IconStyle>>(id.style), seed);
    }
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
inline size_t qHash(const QColor &color, size_t seed = 0) { return qHash(color.rgba(), seed); }
#else
inline uint qHash(const QColor &color, uint seed = 0) noexcept { return qHash(color.rgba(), seed); }
#endif

#endif //QWIDGET_FANCYUI_HASH_H
