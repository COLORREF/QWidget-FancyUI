//
// Created by TX on 2025/8/8.
//

#include "Palette.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "Core/BezierEasing.h"
#include "Core/Defs.h"
#include "Core/SystemAccessor.h"
#include "Core/Theme/SystemThemeMonitor.h"
#include "Core/Theme/ThemeModeController.h"


namespace fancy
{
    Palette::Palette(QObject *parent) :
        QObject(parent)
    {
        SystemThemeMonitor::monitor().start();

        // 默认颜色组
        _group_theme = {ColorGroups::System, ThemeModeController::controller().appTheme()};
        loadSchemes(QColor::fromString("#D44E7D"));

        connect(&ThemeModeController::controller(), &ThemeModeController::appThemeChange, this, &Palette::onAppThemeChanged);
        connect(&SystemThemeMonitor::monitor(), &SystemThemeMonitor::systemAccentColorsChanged, this, &Palette::onSystemAccentColorsChanged);
    }

    Palette &Palette::palette()
    {
        static auto *p = new Palette(qApp);
        return *p;
    }

    Palette::~Palette()
    {
        if (SystemThemeMonitor::monitor().isRunning())
            SystemThemeMonitor::monitor().stop();
        SystemThemeMonitor::monitor().wait();
    }

    const QHash<Palette::ColorGroup, Palette::ColorsHash> &Palette::colors()
    {
        return _schemes;
    }

    const QColor &Palette::color(const ColorRole role)
    {
        return _schemes[_group_theme][role];
    }

    const QColor &Palette::color(ColorRole role, Theme theme)
    {
        return _schemes[{_group_theme.first, theme}][role];
    }

    const QColor &Palette::operator[](const ColorRole role)
    {
        return _schemes[_group_theme][role];
    }

    void Palette::setColorGroups(ColorGroups group, const QColor &custom)
    {
        _group_theme = {group, ThemeModeController::controller().appTheme()};
        setCustomColor(custom);
    }

    void Palette::setCustomColor(const QColor &color)
    {
        if (_group_theme.first == ColorGroups::Custom)
        {
            if (color.isValid())
                loadSchemes(color);
            else
                loadSchemes(QColor(212, 78, 125));
            emit appColorChange();
        }
    }

    QColor Palette::mix(const QColor &c1, const QColor &c2, double amount)
    {
        amount = qBound(0.0, amount, 1.0);
        if (amount == 0.0)
            return c1;
        if (amount == 1.0)
            return c2;
        return QColor::fromRgbF(
            c1.redF() + (c2.redF() - c1.redF()) * amount,
            c1.greenF() + (c2.greenF() - c1.greenF()) * amount,
            c1.blueF() + (c2.blueF() - c1.blueF()) * amount,
            c1.alphaF() + (c2.alphaF() - c1.alphaF()) * amount
        );
    }

    QColor Palette::darken(const QColor &c, double amount)
    {
        QColor hsl = c.toHsl();
        const double l = qBound(0.0, hsl.lightnessF() - amount, 1.0);
        hsl.setHslF(hsl.hueF(), hsl.saturationF(), l, hsl.alphaF());
        return hsl;
    }

    QColor Palette::spin(const QColor &c, double amount)
    {
        float h, s, l, alpha;
        c.getHslF(&h, &s, &l, &alpha);
        h = std::fmod(h * 360.0 + amount, 360.0);
        return QColor::fromHslF((h < 0 ? h + 360.0 : h) / 360.0, s, l, alpha);
    }

    double Palette::relativeLuminance(const QColor &c, double gamma)
    {
        return 0.2126729 * gammaDecode(c.redF(), gamma) +
               0.7151522 * gammaDecode(c.greenF(), gamma) +
               0.0721750 * gammaDecode(c.blueF(), gamma);
    }

    double Palette::wcag2Contrast(const QColor &c1, const QColor &c2, double gamma)
    {
        double l1 = relativeLuminance(c1, gamma);
        double l2 = relativeLuminance(c2, gamma);
        return l1 == l2 ? 1.0 : l2 < l1 ? (l1 + 0.05) / (l2 + 0.05) : (l2 + 0.05) / (l1 + 0.05);
    }

    int Palette::apcaContrast(const QColor &bg, const QColor &text)
    {
        constexpr double mainTRC = 2.4; // sRGB 传输曲线指数
        constexpr double normBG = 0.56; // 正常极性：背景幂次
        constexpr double normTXT = 0.57; // 正常极性：文字幂次
        constexpr double revTXT = 0.62; // 反转极性：文字幂次
        constexpr double revBG = 0.65; // 反转极性：背景幂次
        constexpr double blkThrs = 0.022; // 黑位阈值（soft clamp 起始）
        constexpr double blkClmp = 1.414; // 黑位夹钳指数
        constexpr double scaleBoW = 1.14; // 正常极性缩放
        constexpr double scaleWoB = 1.14; // 反转极性缩放
        constexpr double deltaYmin = 0.0005; // 极小差异阈值（避免除零/噪声）
        constexpr double Wclamp = 0.1;
        constexpr double Woffset = 0.027; // 反转极性：偏置

        auto calcAPCAcontrast = [](const QColor &textColor, const QColor &bgColor)-> int {
            auto softclp = [](double Yc)-> double {
                if (Yc < 0)
                    return 0;
                return Yc <= blkThrs ? Yc + std::pow((blkThrs - Yc), blkClmp) : Yc;
            };

            auto SAPC = [](double Ybg, double Ytxt)-> double {
                if (Ybg > Ytxt)
                    return (std::pow(Ybg, normBG) - std::pow(Ytxt, normTXT)) * scaleBoW;
                return (std::pow(Ybg, revBG) - std::pow(Ytxt, revTXT)) * scaleWoB;
            };

            auto Lc = [](double sapc)-> int {
                if (qFabs(sapc) < Wclamp)
                    return 0.0;
                return static_cast<int>(std::lround(sapc > 0.0 ? (sapc - Woffset) * 100.0 : (sapc + Woffset) * 100.0));
            };

            double Ytxt = softclp(relativeLuminance(textColor, mainTRC));
            double Ybg = softclp(relativeLuminance(bgColor, mainTRC));
            if (qFabs(Ybg - Ytxt) < deltaYmin)
                return 0;
            return Lc(SAPC(Ybg, Ytxt));
        };

        return calcAPCAcontrast(text, bg);
    }

    QColor Palette::apcaTextColor(const QColor &bg)
    {
        int LcBlack = apcaContrast(Qt::GlobalColor::black, bg);
        int LcWhite = apcaContrast(Qt::GlobalColor::white, bg);
        return qAbs(LcBlack) > qAbs(LcWhite) ? Qt::GlobalColor::black : Qt::GlobalColor::white;
    }

    QColor Palette::colorPalette(const QColor &c, int index, int max)
    {
        constexpr double warmDark = 0.5; // 暖色变暗系数
        constexpr double warmRotate = -26.0; // 暖色旋转角度
        constexpr double coldDark = 0.55; // 冷色变暗系数
        constexpr double coldRotate = 10.0; // 冷色旋转角度

        // 贝塞尔曲线控制点坐标
        constexpr double x1 = 0.26;
        constexpr double x2 = 0.09;
        constexpr double y1 = 0.37;
        constexpr double y2 = 0.18;

        static BezierEasing colorEasing(x1, y1, x2, y2);
        const int midIndex = max / 2 + 1;

        // 根据颜色特性生成的阴影颜色
        auto getShadeColor = [](const QColor &ori)-> QColor {
            // 暖色
            if (ori.redF() > ori.blueF())
                return spin(darken(ori, ori.lightnessF() * warmDark), warmRotate);
            // 冷色
            return spin(darken(ori, ori.lightnessF() * coldDark), coldRotate);
        };

        const double currentEasing = colorEasing(static_cast<double>(index) * (1.0 / static_cast<double>(max)));
        const double primaryEasing = colorEasing(static_cast<double>(midIndex) / static_cast<double>(max)); // 分界点

        // 较浅的色调（与白色混合）
        if (index <= midIndex)
            return mix(Qt::GlobalColor::white, c, currentEasing / primaryEasing);

        // 较深的色调（与阴影色混合)
        return mix(getShadeColor(c), c, 1 - (currentEasing - primaryEasing) / (1 - primaryEasing));
    }

    QList<QColor> Palette::antDesign2ColorPalettes(const QColor &base)
    {
        QList<QColor> colors;
        for (int index = 1; index <= 10; index++)
            colors.append(colorPalette(base, index, 10));
        return colors;
    }

    void Palette::loadSchemes(const QColor &custom)
    {
        const QJsonObject group_obj = loadColorProfile(_group_theme.first);
        static const QStringList theme_names = {"Light", "Dark"}; // 预定义的主题名称列表
        for (const QString &theme_name: theme_names) // 遍历每个主题
        {
            // ------------------------------------------------------------
            // 1. 将主题名称字符串（如 "Light"）转换为枚举值 Theme
            //    magic_enum::enum_cast 返回 std::optional<Theme>
            // ------------------------------------------------------------
            auto theme_enum = magic_enum::enum_cast<Theme>(theme_name.toStdString());
            // 如果枚举转换失败（无效名称），或 group_obj 中不存在该主题键，则跳过
            if (!theme_enum.has_value() || !group_obj.contains(theme_name))
                continue;

            // 获取当前主题下的 json 对象（包含多个 ColorRole 的键值对）
            const QJsonObject theme_obj = group_obj[theme_name].toObject();

            // 获取该颜色组 + 主题对应的方案存储引用
            ColorsHash &scheme = _schemes[{_group_theme.first, theme_enum.value()}];

            // ------------------------------------------------------------
            // 2. 记录需要延迟计算的 ColorRole，以 "$calc:" 开头（用户自定义颜色）
            //    这些颜色依赖其他已解析的颜色值
            // ------------------------------------------------------------
            QList<ColorRole> deferred_roles;

            // 第1次遍历，解析直接颜色值
            for (auto it = theme_obj.begin(); it != theme_obj.end(); ++it)
            {
                // 将 json 键转换为 ColorRole 枚举
                auto role_enum = magic_enum::enum_cast<ColorRole>(it.key().toStdString());

                // 枚举转换失败，说明 json 中有未知的角色名，记录警告并跳过
                if (!role_enum.has_value())
                {
                    qWarning() << "Palette: Unknown ColorRole in JSON:" << it.key();
                    continue;
                }
                const QString raw = it.value().toString(); // 获取原始颜色值字符串

                // 如果值以 "$calc:" 开头，说明需要运行时计算，先加入延迟列表，第二轮处理
                if (raw.startsWith("$calc:"))
                {
                    deferred_roles.append(role_enum.value());
                    continue;
                }
                scheme[role_enum.value()] = parseColorFromJsonStr(custom, raw); // 解析颜色，并存储
            }

            // 第2次遍遍历：处理所有延迟计算的颜色
            for (const ColorRole &role: deferred_roles)
            {
                switch (role)
                {
                    case ColorRole::AppAccentText :
                        if (scheme.contains(ColorRole::AppAccent)) // AppAccentText 需要依赖已经解析好的 AppAccent 颜色
                            scheme[role] = apcaTextColor(scheme[ColorRole::AppAccent]);
                        else
                            qWarning() << "Palette: AppAccent not resolved before AppAccentText"; // 如果 AppAccent 未被解析，无法计算，输出警告
                        break;
                    default : // 其他 $calc 角色暂不支持，输出警告并显示角色名
                        qWarning() << "Palette: Unsupported $calc role:" << magic_enum::enum_name(role).data();
                        break;
                }
            }
        }
    }

    QJsonObject Palette::loadColorProfile(ColorGroups group)
    {
        QFile file(":/Profiles/Palette/ColorProfile.json");
        if (!file.open(QFile::ReadOnly))
        {
            qCritical() << "Failed to open color profile file:" << file.errorString();
            return QJsonObject(); // 错误：返回空对象
        }

        // 解析 json
        QJsonParseError parse_error;
        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parse_error);
        file.close();
        if (parse_error.error != QJsonParseError::NoError)
        {
            qCritical() << "JSON parse error in color profile:" << parse_error.errorString() << "at offset" << parse_error.offset;
            return QJsonObject();
        }
        if (!doc.isObject())
        {
            qCritical() << "Color profile JSON root is not an object";
            return QJsonObject();
        }
        const QJsonObject root = doc.object();

        // 从颜色组枚举获取 json 对象
        const auto group_name_view = magic_enum::enum_name(group);
        if (group_name_view.empty())
        {
            qCritical() << "Invalid enum value for group theme:" << static_cast<std::underlying_type_t<decltype(group)>>(group);
            return QJsonObject();
        }
        const QString group_name = QString::fromUtf8(group_name_view.data(), static_cast<int>(group_name_view.size()));

        if (!root.contains(group_name)) // 从根对象中提取对应的组对象
        {
            qWarning() << "Group" << group_name << "not found in color profile, using empty object";
            return QJsonObject();
        }
        return root[group_name].toObject(); // 成功返回
    }

    QColor Palette::parseColorFromJsonStr(const QColor &custom, const QString &raw)
    {
        const QMap<SysAccentPalette, QColor> sys_colors = SystemAccessor::sysAccentColors();

        // 套用 SysAccentPalette 枚举（含义一样），存储自定义颜色及其衍生色
        QMap<SysAccentPalette, QColor> cus_colors;
        QList<QColor> custom_colors = antDesign2ColorPalettes(custom);
        for (int i = 0; i < 7; i++) // 只取7个颜色
            cus_colors[static_cast<SysAccentPalette>(i)] = custom_colors[i + 2];

        // 十六进制颜色，以 # 开头，去掉开头 #，保留后面的十六进制字符串
        if (const QString hex = raw.mid(1); raw.startsWith("#"))
        {
            bool ok = false; // 用于判断字符串转整数是否成功

            // 从 hex 中提取 R, G, B 分量，每分量占 2 位十六进制
            const int r = hex.mid(0, 2).toInt(&ok, 16); // 从位置0开始取2个字符转，由16进制转10进制整数
            const int g = hex.mid(2, 2).toInt(&ok, 16); // 从位置2开始
            const int b = hex.mid(4, 2).toInt(&ok, 16); // 从位置4开始
            if (const int a = hex.mid(6, 2).toInt(&ok, 16); hex.length() == 8) // hex 长度为 8，包含 Alpha 通道
                return QColor(r, g, b, a);
            return QColor(r, g, b);
        }

        // 系统调色板引用，以 "$sys:" 开头
        if (raw.startsWith("$sys:"))
        {
            // 提取枚举字符串并转换，从系统颜色映射表中取出对应颜色
            if (const auto accent = magic_enum::enum_cast<SysAccentPalette>(raw.mid(5).toStdString()); accent.has_value())
                return sys_colors.value(accent.value());
            qWarning() << "Palette: Unknown $sys key:" << raw;
            return QColor();
        }

        // 用户自定义调色板引用，以 "$cus:" 开头
        if (raw.startsWith(QLatin1String("$cus:")))
        {
            // 提取枚举名并转换，从用户颜色映射表取值
            if (const auto accent = magic_enum::enum_cast<SysAccentPalette>(raw.mid(5).toStdString()); accent.has_value())
                return cus_colors.value(accent.value());
            qWarning() << "Palette: Unknown $cus key:" << raw;
            return QColor();
        }

        // 所有格式都不匹配
        qWarning() << "Palette: Unrecognized value format:" << raw;
        return QColor();
    }

    void Palette::onAppThemeChanged(Theme theme)
    {
        _group_theme = {_group_theme.first, theme};
        emit appThemeChange();
    }

    void Palette::onSystemAccentColorsChanged(QMap<SysAccentPalette, QColor> colors)
    {
        const auto froup = _group_theme.first;
        if (froup == ColorGroups::System)
        {
            ColorsHash &sys_light_scheme = _schemes[{ColorGroups::System, Theme::Light}];
            ColorsHash &sys_dark_scheme = _schemes[{ColorGroups::System, Theme::Dark}];

            sys_light_scheme[ColorRole::Link] = colors[SysAccentPalette::AccentLight2];
            sys_light_scheme[ColorRole::ImportantLink] = colors[SysAccentPalette::AccentDark2];
            sys_light_scheme[ColorRole::AppAccent] = colors[SysAccentPalette::AccentDark1];
            sys_light_scheme[ColorRole::AppAccentText] = apcaTextColor(sys_light_scheme[ColorRole::AppAccent]);
            sys_light_scheme[ColorRole::SysAccent] = colors[SysAccentPalette::Accent];

            sys_dark_scheme[ColorRole::Link] = colors[SysAccentPalette::AccentLight3];
            sys_dark_scheme[ColorRole::ImportantLink] = colors[SysAccentPalette::AccentLight3];
            sys_dark_scheme[ColorRole::AppAccent] = colors[SysAccentPalette::AccentLight2];
            sys_dark_scheme[ColorRole::AppAccentText] = apcaTextColor(sys_dark_scheme[ColorRole::AppAccent]);
            sys_dark_scheme[ColorRole::SysAccent] = colors[SysAccentPalette::Accent];
        }
        else if (froup == ColorGroups::Default)
        {
            ColorsHash &def_light_scheme = _schemes[{ColorGroups::Default, Theme::Light}];
            ColorsHash &def_dark_scheme = _schemes[{ColorGroups::Default, Theme::Dark}];

            def_light_scheme[ColorRole::SysAccent] = colors[SysAccentPalette::Accent];
            def_dark_scheme[ColorRole::SysAccent] = colors[SysAccentPalette::Accent];
        }
        if (froup != ColorGroups::Custom)
            emit appColorChange();
    }
} // fancy
