//
// Created by TX on 2025/8/8.
//

#include "Palette.h"

#include <QApplication>

#include "BezierEasing.h"
#include "Defs.h"
#include "SystemAccessor.h"
#include "SystemThemeMonitor.h"
#include "ThemeModeController.h"


namespace fancy
{
    Palette::Palette(QObject *parent) :
        QObject(parent)
    {
        SystemThemeMonitor::monitor().start();

        // 默认颜色组
        _group_theme = {ColorGroups::System, ThemeModeController::controller().appTheme()};
        loadSchemes(QColor(212, 78, 125));

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
        QMap<SysAccentPalette, QColor> sys_colors = SystemAccessor::sysAccentColors();

        // 套用SysAccentPalette枚举（含义一样），存储自定义颜色及其衍生色
        QMap<SysAccentPalette, QColor> cus_colors;
        QList<QColor> custom_colors = antDesign2ColorPalettes(custom);
        for (int i = 0; i < 7; i++)
            cus_colors[static_cast<SysAccentPalette>(i)] = custom_colors[i + 2];

        switch (_group_theme.first)
        {
            case ColorGroups::System :
            {
                ColorsHash &sys_light_scheme = _schemes[{ColorGroups::System, Theme::Light}];
                ColorsHash &sys_dark_scheme = _schemes[{ColorGroups::System, Theme::Dark}];

                sys_light_scheme[ColorRole::Window] = QColor(243, 243, 243);
                sys_light_scheme[ColorRole::WindowText] = Qt::GlobalColor::black;
                sys_light_scheme[ColorRole::WindowBright] = QColor(196, 43, 28);
                sys_light_scheme[ColorRole::Border] = QColor(131, 131, 131);
                sys_light_scheme[ColorRole::Text] = Qt::GlobalColor::black;
                sys_light_scheme[ColorRole::Button] = QColor(252, 252, 252);
                sys_light_scheme[ColorRole::ButtonBorder] = QColor(217, 217, 217);
                sys_light_scheme[ColorRole::ButtonBottomLine] = QColor(211, 211, 211);
                sys_light_scheme[ColorRole::RadioButtonInnerCircle] = QColor(243, 243, 243);
                sys_light_scheme[ColorRole::Link] = sys_colors[SysAccentPalette::AccentLight2];
                sys_light_scheme[ColorRole::ImportantLink] = sys_colors[SysAccentPalette::AccentDark2];
                sys_light_scheme[ColorRole::LinkVisited] = QColor(149, 79, 114);
                sys_light_scheme[ColorRole::DisEnabled] = QColor(109, 109, 109);
                sys_light_scheme[ColorRole::DisEnabledText] = QColor(0, 0, 0, 64);
                sys_light_scheme[ColorRole::AppAccent] = sys_colors[SysAccentPalette::AccentDark1];
                sys_light_scheme[ColorRole::SysAccent] = sys_colors[SysAccentPalette::Accent];
                sys_light_scheme[ColorRole::AppAccentText] = apcaTextColor(sys_light_scheme[ColorRole::AppAccent]);
                sys_light_scheme[ColorRole::MaskNormal] = Qt::GlobalColor::transparent;
                sys_light_scheme[ColorRole::MaskHover] = QColor(0, 0, 0, 10);
                sys_light_scheme[ColorRole::MaskPressed] = QColor(0, 0, 0, 6);
                sys_light_scheme[ColorRole::MaskBrightPressed] = QColor(230, 220, 255, 24);
                sys_light_scheme[ColorRole::MaskSelected] = QColor(90, 90, 90, 15);
                sys_light_scheme[ColorRole::MaskSelectedHover] = QColor(73, 73, 73, 9);
                sys_light_scheme[ColorRole::MaskSelectedPressed] = QColor(90, 90, 90, 15);
                sys_light_scheme[ColorRole::MSWindow] = QColor(252, 252, 252, 170);
                sys_light_scheme[ColorRole::MSWindowSplitLine] = QColor(229, 229, 229);
                sys_light_scheme[ColorRole::ScrollBarNormal] = Qt::GlobalColor::transparent;
                sys_light_scheme[ColorRole::ScrollBarFocus] = QColor(249, 249, 249);
                sys_light_scheme[ColorRole::ScrollBarEndArrow] = QColor(138, 138, 138);
                sys_light_scheme[ColorRole::ScrollBarSlider] = QColor(138, 138, 138);
                sys_light_scheme[ColorRole::TextEditPanelNormal] = QColor(251, 251, 251);
                sys_light_scheme[ColorRole::TextEditPanelFocus] = QColor(255, 255, 255);
                sys_light_scheme[ColorRole::TextEditPanelHover] = QColor(246, 246, 246);
                sys_light_scheme[ColorRole::TextEditBorder] = QColor(229, 229, 229);
                sys_light_scheme[ColorRole::TextEditIndicatorLine] = QColor(134, 134, 134);
                sys_light_scheme[ColorRole::PlaceholderText] = QColor(95, 95, 95);

                sys_dark_scheme[ColorRole::Window] = QColor(32, 32, 32);
                sys_dark_scheme[ColorRole::WindowText] = Qt::GlobalColor::white;
                sys_dark_scheme[ColorRole::WindowBright] = QColor(196, 43, 28);
                sys_dark_scheme[ColorRole::Border] = QColor(131, 131, 131);
                sys_dark_scheme[ColorRole::Text] = Qt::GlobalColor::white;
                sys_dark_scheme[ColorRole::Button] = QColor(55, 55, 55);
                sys_dark_scheme[ColorRole::ButtonBorder] = QColor(66, 66, 66);
                sys_dark_scheme[ColorRole::ButtonBottomLine] = QColor(44, 44, 44);
                sys_dark_scheme[ColorRole::RadioButtonInnerCircle] = QColor(32, 32, 32);
                sys_dark_scheme[ColorRole::Link] = sys_colors[SysAccentPalette::AccentLight3];
                sys_dark_scheme[ColorRole::ImportantLink] = sys_colors[SysAccentPalette::AccentLight3];
                sys_dark_scheme[ColorRole::LinkVisited] = QColor(224, 158, 188);
                sys_dark_scheme[ColorRole::DisEnabled] = QColor(180, 180, 180);
                sys_dark_scheme[ColorRole::DisEnabledText] = QColor(255, 255, 255, 64);
                sys_dark_scheme[ColorRole::AppAccent] = sys_colors[SysAccentPalette::AccentLight2];
                sys_dark_scheme[ColorRole::SysAccent] = sys_colors[SysAccentPalette::Accent];
                sys_dark_scheme[ColorRole::AppAccentText] = apcaTextColor(sys_dark_scheme[ColorRole::AppAccent]);
                sys_dark_scheme[ColorRole::MaskNormal] = Qt::GlobalColor::transparent;
                sys_dark_scheme[ColorRole::MaskHover] = QColor(255, 255, 255, 15);
                sys_dark_scheme[ColorRole::MaskPressed] = QColor(255, 255, 255, 10);
                sys_dark_scheme[ColorRole::MaskBrightPressed] = QColor(0, 0, 18, 22);
                sys_dark_scheme[ColorRole::MaskSelected] = QColor(253, 253, 253, 15);
                sys_dark_scheme[ColorRole::MaskSelectedHover] = QColor(185, 185, 185, 15);
                sys_dark_scheme[ColorRole::MaskSelectedPressed] = QColor(253, 253, 253, 15);
                sys_dark_scheme[ColorRole::MSWindow] = QColor(151, 151, 151, 15);
                sys_dark_scheme[ColorRole::MSWindowSplitLine] = QColor(15, 15, 15, 45);
                sys_dark_scheme[ColorRole::ScrollBarNormal] = Qt::GlobalColor::transparent;
                sys_dark_scheme[ColorRole::ScrollBarFocus] = QColor(44, 44, 44);
                sys_dark_scheme[ColorRole::ScrollBarEndArrow] = QColor(159, 159, 159);
                sys_dark_scheme[ColorRole::ScrollBarSlider] = QColor(159, 159, 159);
                sys_dark_scheme[ColorRole::TextEditPanelNormal] = QColor(45, 45, 45);
                sys_dark_scheme[ColorRole::TextEditPanelFocus] = QColor(31, 31, 31);
                sys_dark_scheme[ColorRole::TextEditPanelHover] = QColor(50, 50, 50);
                sys_dark_scheme[ColorRole::TextEditBorder] = QColor(48, 48, 48);
                sys_dark_scheme[ColorRole::TextEditIndicatorLine] = QColor(154, 154, 154);
                sys_dark_scheme[ColorRole::PlaceholderText] = QColor(207, 207, 207);
            }
            break;
            case ColorGroups::Default :
            {
                custom_colors = antDesign2ColorPalettes(QColor(212, 78, 125));
                for (int i = 0; i < 7; i++)
                    cus_colors[static_cast<SysAccentPalette>(i)] = custom_colors[i + 2];

                ColorsHash &def_light_scheme = _schemes[{ColorGroups::Default, Theme::Light}];
                ColorsHash &def_dark_scheme = _schemes[{ColorGroups::Default, Theme::Dark}];

                def_light_scheme[ColorRole::Window] = QColor(240, 243, 249);
                def_light_scheme[ColorRole::WindowText] = Qt::GlobalColor::black;
                def_light_scheme[ColorRole::WindowBright] = QColor(196, 43, 28);
                def_light_scheme[ColorRole::Border] = QColor(160, 160, 160);
                def_light_scheme[ColorRole::Text] = Qt::GlobalColor::black;
                def_light_scheme[ColorRole::Button] = QColor(252, 252, 252);
                def_light_scheme[ColorRole::ButtonBorder] = QColor(217, 217, 217);
                def_light_scheme[ColorRole::ButtonBottomLine] = QColor(211, 211, 211);
                def_light_scheme[ColorRole::RadioButtonInnerCircle] = QColor(240, 243, 249);
                def_light_scheme[ColorRole::Link] = cus_colors[SysAccentPalette::AccentLight2];
                def_light_scheme[ColorRole::ImportantLink] = cus_colors[SysAccentPalette::AccentDark2];
                def_light_scheme[ColorRole::LinkVisited] = QColor(212, 98, 78);
                def_light_scheme[ColorRole::DisEnabled] = QColor(109, 109, 109);
                def_light_scheme[ColorRole::DisEnabledText] = QColor(0, 0, 0, 64);
                def_light_scheme[ColorRole::AppAccent] = cus_colors[SysAccentPalette::AccentDark1];
                def_light_scheme[ColorRole::SysAccent] = sys_colors[SysAccentPalette::Accent];
                def_light_scheme[ColorRole::AppAccentText] = apcaTextColor(def_light_scheme[ColorRole::AppAccent]);
                def_light_scheme[ColorRole::MaskNormal] = Qt::GlobalColor::transparent;
                def_light_scheme[ColorRole::MaskHover] = QColor(0, 0, 0, 10);
                def_light_scheme[ColorRole::MaskPressed] = QColor(0, 0, 0, 6);
                def_light_scheme[ColorRole::MaskBrightPressed] = QColor(230, 220, 255, 24);
                def_light_scheme[ColorRole::MaskSelected] = QColor(90, 90, 90, 15);
                def_light_scheme[ColorRole::MaskSelectedHover] = QColor(73, 73, 73, 9);
                def_light_scheme[ColorRole::MaskSelectedPressed] = QColor(90, 90, 90, 15);
                def_light_scheme[ColorRole::MSWindow] = QColor(252, 252, 252, 170);
                def_light_scheme[ColorRole::MSWindowSplitLine] = QColor(229, 229, 229);
                def_light_scheme[ColorRole::ScrollBarNormal] = Qt::GlobalColor::transparent;
                def_light_scheme[ColorRole::ScrollBarFocus] = QColor(249, 249, 249);
                def_light_scheme[ColorRole::ScrollBarEndArrow] = QColor(138, 138, 138);
                def_light_scheme[ColorRole::ScrollBarSlider] = QColor(138, 138, 138);
                def_light_scheme[ColorRole::TextEditPanelNormal] = QColor(251, 251, 251);
                def_light_scheme[ColorRole::TextEditPanelFocus] = QColor(255, 255, 255);
                def_light_scheme[ColorRole::TextEditPanelHover] = QColor(246, 246, 246);
                def_light_scheme[ColorRole::TextEditBorder] = QColor(229, 229, 229);
                def_light_scheme[ColorRole::TextEditIndicatorLine] = QColor(134, 134, 134);
                def_light_scheme[ColorRole::PlaceholderText] = QColor(95, 95, 95);

                def_dark_scheme[ColorRole::Window] = QColor(26, 32, 52);
                def_dark_scheme[ColorRole::WindowText] = Qt::GlobalColor::white;
                def_dark_scheme[ColorRole::WindowBright] = QColor(196, 43, 28);
                def_dark_scheme[ColorRole::Border] = QColor(131, 131, 131);
                def_dark_scheme[ColorRole::Text] = Qt::GlobalColor::white;
                def_dark_scheme[ColorRole::Button] = QColor(55, 55, 55);
                def_dark_scheme[ColorRole::ButtonBorder] = QColor(66, 66, 66);
                def_dark_scheme[ColorRole::ButtonBottomLine] = QColor(44, 44, 44);
                def_dark_scheme[ColorRole::RadioButtonInnerCircle] = QColor(26, 32, 52);
                def_dark_scheme[ColorRole::Link] = cus_colors[SysAccentPalette::AccentLight3];
                def_dark_scheme[ColorRole::ImportantLink] = cus_colors[SysAccentPalette::AccentLight3];
                def_dark_scheme[ColorRole::LinkVisited] = QColor(212, 98, 78);
                def_dark_scheme[ColorRole::DisEnabled] = QColor(180, 180, 180);
                def_dark_scheme[ColorRole::DisEnabledText] = QColor(255, 255, 255, 64);
                def_dark_scheme[ColorRole::AppAccent] = cus_colors[SysAccentPalette::AccentLight2];
                def_dark_scheme[ColorRole::SysAccent] = sys_colors[SysAccentPalette::Accent];
                def_dark_scheme[ColorRole::AppAccentText] = apcaTextColor(def_dark_scheme[ColorRole::AppAccent]);
                def_dark_scheme[ColorRole::MaskNormal] = Qt::GlobalColor::transparent;
                def_dark_scheme[ColorRole::MaskHover] = QColor(255, 255, 255, 15);
                def_dark_scheme[ColorRole::MaskPressed] = QColor(255, 255, 255, 10);
                def_dark_scheme[ColorRole::MaskBrightPressed] = QColor(0, 0, 18, 22);
                def_dark_scheme[ColorRole::MaskSelected] = QColor(253, 253, 253, 15);
                def_dark_scheme[ColorRole::MaskSelectedHover] = QColor(185, 185, 185, 15);
                def_dark_scheme[ColorRole::MaskSelectedPressed] = QColor(253, 253, 253, 15);
                def_dark_scheme[ColorRole::MSWindow] = QColor(151, 151, 151, 15);
                def_dark_scheme[ColorRole::MSWindowSplitLine] = QColor(15, 15, 15, 45);
                def_dark_scheme[ColorRole::ScrollBarNormal] = Qt::GlobalColor::transparent;
                def_dark_scheme[ColorRole::ScrollBarFocus] = QColor(44, 44, 44);
                def_dark_scheme[ColorRole::ScrollBarEndArrow] = QColor(159, 159, 159);
                def_dark_scheme[ColorRole::ScrollBarSlider] = QColor(159, 159, 159);
                def_dark_scheme[ColorRole::TextEditPanelNormal] = QColor(45, 45, 45);
                def_dark_scheme[ColorRole::TextEditPanelFocus] = QColor(31, 31, 31);
                def_dark_scheme[ColorRole::TextEditPanelHover] = QColor(50, 50, 50);
                def_dark_scheme[ColorRole::TextEditBorder] = QColor(48, 48, 48);
                def_dark_scheme[ColorRole::TextEditIndicatorLine] = QColor(154, 154, 154);
                def_dark_scheme[ColorRole::PlaceholderText] = QColor(207, 207, 207);
            }
            break;
            case ColorGroups::Custom :
            {
                ColorsHash &cus_light_scheme = _schemes[{ColorGroups::Custom, Theme::Light}];
                ColorsHash &cus_dark_scheme = _schemes[{ColorGroups::Custom, Theme::Dark}];

                cus_light_scheme[ColorRole::Window] = QColor(243, 243, 243);
                cus_light_scheme[ColorRole::WindowText] = Qt::GlobalColor::black;
                cus_light_scheme[ColorRole::WindowBright] = QColor(196, 43, 28);
                cus_light_scheme[ColorRole::Border] = QColor(131, 131, 131);
                cus_light_scheme[ColorRole::Text] = Qt::GlobalColor::black;
                cus_light_scheme[ColorRole::Button] = QColor(252, 252, 252);
                cus_light_scheme[ColorRole::ButtonBorder] = QColor(217, 217, 217);
                cus_light_scheme[ColorRole::ButtonBottomLine] = QColor(211, 211, 211);
                cus_light_scheme[ColorRole::RadioButtonInnerCircle] = QColor(243, 243, 243);
                cus_light_scheme[ColorRole::Link] = cus_colors[SysAccentPalette::AccentLight2];
                cus_light_scheme[ColorRole::ImportantLink] = cus_colors[SysAccentPalette::AccentDark2];
                cus_light_scheme[ColorRole::LinkVisited] = QColor(149, 79, 114);
                cus_light_scheme[ColorRole::DisEnabled] = QColor(109, 109, 109);
                cus_light_scheme[ColorRole::DisEnabledText] = QColor(0, 0, 0, 64);
                cus_light_scheme[ColorRole::AppAccent] = cus_colors[SysAccentPalette::AccentDark1];
                cus_light_scheme[ColorRole::SysAccent] = sys_colors[SysAccentPalette::Accent];
                cus_light_scheme[ColorRole::AppAccentText] = apcaTextColor(cus_light_scheme[ColorRole::AppAccent]);
                cus_light_scheme[ColorRole::MaskNormal] = Qt::GlobalColor::transparent;
                cus_light_scheme[ColorRole::MaskHover] = QColor(0, 0, 0, 10);
                cus_light_scheme[ColorRole::MaskPressed] = QColor(0, 0, 0, 6);
                cus_light_scheme[ColorRole::MaskBrightPressed] = QColor(230, 220, 255, 24);
                cus_light_scheme[ColorRole::MaskSelected] = QColor(90, 90, 90, 15);
                cus_light_scheme[ColorRole::MaskSelectedHover] = QColor(73, 73, 73, 9);
                cus_light_scheme[ColorRole::MaskSelectedPressed] = QColor(90, 90, 90, 15);
                cus_light_scheme[ColorRole::MSWindow] = QColor(252, 252, 252, 170);
                cus_light_scheme[ColorRole::MSWindowSplitLine] = QColor(229, 229, 229);
                cus_light_scheme[ColorRole::ScrollBarNormal] = Qt::GlobalColor::transparent;
                cus_light_scheme[ColorRole::ScrollBarFocus] = QColor(249, 249, 249);
                cus_light_scheme[ColorRole::ScrollBarEndArrow] = QColor(138, 138, 138);
                cus_light_scheme[ColorRole::ScrollBarSlider] = QColor(138, 138, 138);
                cus_light_scheme[ColorRole::TextEditPanelNormal] = QColor(251, 251, 251);
                cus_light_scheme[ColorRole::TextEditPanelFocus] = QColor(255, 255, 255);
                cus_light_scheme[ColorRole::TextEditPanelHover] = QColor(246, 246, 246);
                cus_light_scheme[ColorRole::TextEditBorder] = QColor(229, 229, 229);
                cus_light_scheme[ColorRole::TextEditIndicatorLine] = QColor(134, 134, 134);
                cus_light_scheme[ColorRole::PlaceholderText] = QColor(95, 95, 95);

                cus_dark_scheme[ColorRole::Window] = QColor(32, 32, 32);
                cus_dark_scheme[ColorRole::WindowText] = Qt::GlobalColor::white;
                cus_dark_scheme[ColorRole::WindowBright] = QColor(196, 43, 28);
                cus_dark_scheme[ColorRole::Border] = QColor(131, 131, 131);
                cus_dark_scheme[ColorRole::Text] = Qt::GlobalColor::white;
                cus_dark_scheme[ColorRole::Button] = QColor(55, 55, 55);
                cus_dark_scheme[ColorRole::ButtonBorder] = QColor(66, 66, 66);
                cus_dark_scheme[ColorRole::ButtonBottomLine] = QColor(44, 44, 44);
                cus_dark_scheme[ColorRole::RadioButtonInnerCircle] = QColor(32, 32, 32);
                cus_dark_scheme[ColorRole::Link] = cus_colors[SysAccentPalette::AccentLight3];
                cus_dark_scheme[ColorRole::ImportantLink] = cus_colors[SysAccentPalette::AccentLight3];
                cus_dark_scheme[ColorRole::LinkVisited] = QColor(224, 158, 188);
                cus_dark_scheme[ColorRole::DisEnabled] = QColor(180, 180, 180);
                cus_dark_scheme[ColorRole::DisEnabledText] = QColor(255, 255, 255, 64);
                cus_dark_scheme[ColorRole::AppAccent] = cus_colors[SysAccentPalette::AccentLight2];
                cus_dark_scheme[ColorRole::SysAccent] = sys_colors[SysAccentPalette::Accent];
                cus_dark_scheme[ColorRole::AppAccentText] = apcaTextColor(cus_dark_scheme[ColorRole::AppAccent]);
                cus_dark_scheme[ColorRole::MaskNormal] = Qt::GlobalColor::transparent;
                cus_dark_scheme[ColorRole::MaskHover] = QColor(255, 255, 255, 15);
                cus_dark_scheme[ColorRole::MaskPressed] = QColor(255, 255, 255, 10);
                cus_dark_scheme[ColorRole::MaskBrightPressed] = QColor(0, 0, 18, 22);
                cus_dark_scheme[ColorRole::MaskSelected] = QColor(253, 253, 253, 15);
                cus_dark_scheme[ColorRole::MaskSelectedHover] = QColor(185, 185, 185, 15);
                cus_dark_scheme[ColorRole::MaskSelectedPressed] = QColor(253, 253, 253, 15);
                cus_dark_scheme[ColorRole::MSWindow] = QColor(151, 151, 151, 15);
                cus_dark_scheme[ColorRole::MSWindowSplitLine] = QColor(15, 15, 15, 45);
                cus_dark_scheme[ColorRole::ScrollBarNormal] = Qt::GlobalColor::transparent;
                cus_dark_scheme[ColorRole::ScrollBarFocus] = QColor(44, 44, 44);
                cus_dark_scheme[ColorRole::ScrollBarEndArrow] = QColor(159, 159, 159);
                cus_dark_scheme[ColorRole::ScrollBarSlider] = QColor(159, 159, 159);
                cus_dark_scheme[ColorRole::TextEditPanelNormal] = QColor(45, 45, 45);
                cus_dark_scheme[ColorRole::TextEditPanelFocus] = QColor(31, 31, 31);
                cus_dark_scheme[ColorRole::TextEditPanelHover] = QColor(50, 50, 50);
                cus_dark_scheme[ColorRole::TextEditBorder] = QColor(48, 48, 48);
                cus_dark_scheme[ColorRole::TextEditIndicatorLine] = QColor(154, 154, 154);
                cus_dark_scheme[ColorRole::PlaceholderText] = QColor(207, 207, 207);
            }
            break;
        }
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
