#include "FancyIcon.h"
#include <string_view>

QString fancyIconPath(FancyIcon iconEnum, Theme::Type themeType)
{
    std::string_view icon_name = magic_enum::enum_name(iconEnum);
    std::string_view theme_name = magic_enum::enum_name(themeType);
    return QString(":/FancyIcon/%1/%2.svg").arg(QString::fromUtf8(theme_name.data(), theme_name.size()),QString::fromUtf8(icon_name.data(), icon_name.size()));
}

QString fancyIconName(FancyIcon iconEnum)
{
    std::string_view icon_name = magic_enum::enum_name(iconEnum);
    return QString::fromUtf8(icon_name.data(), icon_name.size());
}
