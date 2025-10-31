//
// Created by TX on 2025/8/8.
//

#ifndef QWIDGET_FANCYUI_PALETTE_H
#define QWIDGET_FANCYUI_PALETTE_H

#include <QColor>
#include <QHash>
#include <QList>
#include <QMap>
#include <QObject>


namespace fancy
{
    enum class SysAccentPalette;
    enum class Theme;
    enum class ColorGroups;
    enum class ColorRole;

    class Palette final : public QObject
    {
        Q_OBJECT

    public:
        using ColorsHash = QHash<ColorRole, QColor>;
        using ColorGroup = QPair<ColorGroups, Theme>;

        explicit Palette(QObject *parent = nullptr);

        /**
         * @brief 返回全局唯一的调色板对象
         * @return Palettr的引用
         */
        static Palette &palette();

        ~Palette() override;

        [[nodiscard]] const QHash<ColorGroup, ColorsHash> &colors();

        [[nodiscard]] const QColor &color(ColorRole role);

        [[nodiscard]] const QColor &color(ColorRole role, Theme theme);

        [[nodiscard]] const QColor &operator[](ColorRole role); // 等价于 color(ColorRole)

        /**
         * @brief 设置使用的颜色组策略
         * @param group 将要设置的颜色组
         * @param custom 自定义颜色
         * - 仅当颜色组为自定义（ColorGroups::Custom）时 custom 参数生效
         */
        void setColorGroups(ColorGroups group, const QColor &custom = {});

        /**
         * @brief 设置自定义颜色
         * @param color 将要设置的自定义颜色
         * - 仅当颜色组为自定义（ColorGroups::Custom）时函数生效
         */
        void setCustomColor(const QColor &color);

        /**
         * @brief 线性插值颜色混合
         * @param c1 颜色1
         * @param c2 颜色2
         * @param amount 插值参数(0-1)
         * @return 混合后的颜色
         */
        static QColor mix(const QColor &c1, const QColor &c2, float amount = 0.5);

        /**
         * @brief 将给定颜色在HSL色彩空间按指定比例变暗
         * @param c 要变暗的原始颜色
         * @param amount 变暗的程度(0-1)
         * @return 变暗后的新颜色
         */
        static QColor darken(const QColor &c, float amount = 0.1);

        /**
         * @brief 在HSL色彩空间中旋转颜色色相
         * @param c 原始颜色
         * @param amount 色相旋转的角度（单位：度，支持正负值）
         * @return 旋转色相后的新颜色
         */
        static QColor spin(const QColor &c, float amount);

        /**
         * @brief 对srgb颜色空间的单个颜色通道进行伽马解码
         * @param v 输入颜色通道值的伽马编码值 (0-1)(0-255映射)
         * @param gamma 伽马值（默认2.4，sRGB标准等效值）
         * @return 解码后的物理线性光强度(0-1)
         */
        static double gammaDecode(double v, double gamma = 2.4) noexcept { return v <= 0.04045f ? v / 12.92f : std::pow((v + 0.055f) / 1.055f, gamma); }

        /**
         * @brief 计算颜色伽马解码后的感知亮度，该亮度值是符合人眼感知和非线性的
         * @param c 颜色值
         * @param gamma 伽马校正值
         * @return 感知亮度 (0-1)
         */
        static double relativeLuminance(const QColor &c, double gamma = 2.4);

        /**
         * @brief 计算两种颜色之间的对比度（基于 WCAG 2.0 对比度算法）
         * @param c1 第一种颜色
         * @param c2 第二种颜色
         * @param gamma 伽马校正值，用于相对亮度计算
         * @return float 颜色对比度比值（≥1.0）
         */
        static double wcag2Contrast(const QColor &c1, const QColor &c2, float gamma = 2.4);

        /**
         * @brief 基于 APCA 可访问的感知对比度算法计算背景颜色和文本颜色的人眼的实际感受程度
         * @param bg   背景颜色
         * @param text 文本/前景颜色
         * @return 带符号的 APCA Lc（int）。绝对值越大代表感知对比越强
         * @note 计算结果为经过四舍五入后带符号的“感知对比度”整数
         * - 正值：表示 暗字 / 亮底（背景亮度 > 文本亮度）
         * - 负值：表示 亮字 / 暗底（文本亮度 > 背景亮度）
         * @note 返回值说明：
         *  - 0 表示对比低于 APCA 的最小可感知阈值（视为不可读或差异极小）
         *  - 非 0 值通常在约 -108 ~ +106 的范围内（具体取决于函数内部常量参数）
         * @note 推荐用途与最小推荐对比 (|Lc|)
         * - 小号正文字（< 16px 正常字重） |Lc| ≥ 75
         * - 正文常规大小（约 16–18px 正常字重） |Lc| ≥ 60
         * - 较大字体/标题（> 24px 正常字重，或 ≥ 16px 加粗） |Lc| ≥ 45
         * - 装饰/非关键文字 |Lc| ≥ 40
         * - 无关紧要的 UI 辅助性文字（水印、禁用项） |Lc| 可以低至 15–30
         * - 注意：这是方向性阈值，仅供参考
         */
        static int apcaContrast(const QColor &bg, const QColor &text);

        /**
         * @brief 由于WCAG的缺陷，此函数基于APCA算法，文字色默认采用白色和黑色计算，结果采取APCA对比度绝对值高的颜色，它总是返回白色或者黑色
         * @param bg 背景色
         * @return 相对合适的文字颜色（总是为白色或黑色）
         * @note 不保证返回的颜色值一定具有良好的视觉效果或满足无障碍需求
         * - 该返回值只是表明返回的颜色（白色/黑色）相对于另一种颜色（黑色/白色）而言，APCA对比度的绝对值更高，更加合适作为前景色
         * - 若返回值颜色作为文字颜色视觉效果不佳，应当考虑更换背景色
         */
        static QColor apcaTextColor(const QColor &bg);

        /**
         * @brief 基于Ant Design 2.x 的调色板生成算法改写并做了一些拓展，基于给定的基色和索引位置生成对应的调色板颜色
         * - 使用贝塞尔缓动曲线和线性插值颜色混合，初始值的设定与 Ant Design 2.x源码保持一致
         * - 详见：https://github.com/ant-design/ant-design/blob/734beb84ffc3f0469fbae1566aa8450f966cb261/components/style/color/tinyColor.less
         * @param c 基础颜色，作为调色板的参考颜色
         * @param index 颜色在调色板中的位置索引
         * @param max 最大颜色数量，建议值为10
         * @return 生成的调色板颜色
         * @note 确保 0 < index <= max  ！函数内部未作判断
         * @note 循环中调用生成颜色时确保 max 不发生改变
         * @note 建议范围为 max <= 10
         * - 索引1 ~ (int)(max / 2 + 1): 生成比基色更浅的颜色（与白色混合）
         * - 索引(int)(max / 2 + 2) ~ max: 生成比基色更深的颜色（与阴影色混合）
         */
        static QColor colorPalette(const QColor &c, int index, int max = 10);

        /**
         * @brief 对给定的基础色采用 Ant Design 2.x 的调色板生成算法生成 10 种由浅到深的衍生色
         * @param base 基础颜色
         * @return 符合 Ant Design 2.x 的 10 种由浅到深的颜色
         * @note 返回 QList 的元素数量总是为 10
         */
        static QList<QColor> antDesign2ColorPalettes(const QColor &base);

        [[nodiscard]] ColorGroup groupTheme() const { return _group_theme; }

    private:
        void loadSchemes(const QColor &custom = QColor());

    private slots:
        void onAppThemeChanged(Theme theme);

        void onSystemAccentColorsChanged(QMap<SysAccentPalette, QColor> colors);

    private:
        ColorGroup _group_theme;
        QHash<ColorGroup, ColorsHash> _schemes;

    signals:
        void appColorChange();

        void appThemeChange();
    };
} // fancy

#endif //QWIDGET_FANCYUI_PALETTE_H
