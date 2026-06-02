//
// Created by TX on 2025/8/1.
//

#ifndef SYSTEMTHEMEADAPTER_H
#define SYSTEMTHEMEADAPTER_H


#include <QObject>

namespace fancy
{
    enum class Theme;
    /**
     * 主题模式控制器
     * 用于控制应用主题和查询系统主题状态
     * Used to control application themes and query system theme status
     */
    class ThemeModeController final : public QObject
    {
        Q_OBJECT
        friend class SystemThemeMonitor;

        explicit ThemeModeController(QObject *parent = nullptr);

    public:
        ~ThemeModeController() override;

        [[nodiscard]] Theme appTheme() const; // 获取当前应用主题
        [[nodiscard]] Theme systemTheme() const; // 获取当前系统主题
        [[nodiscard]] static ThemeModeController &controller();

        [[nodiscard]] bool isAppLight() const;

        [[nodiscard]] bool isAppDark() const;

        [[nodiscard]] bool isFollowSystem() const;

    public slots:
        void setAppTheme(Theme theme); // 设置应用主题
        void toggleAppLight(); // 切换应用浅色主题
        void toggleAppDark(); // 切换应用深色主题
        void followSystem(); //  切换应用主题跟随系统
        void toggleAppTheme(); // 切换应用主题
    private slots:
        void onSystemThemeChanged(Theme theme);

    private:
        Theme _app_theme; // 应用主题
        Theme _system_theme; // 系统主题
        bool _is_follow_system = true; //是否跟随系统
    signals:
        void appThemeChange(Theme); // 应用主题改变
        void systemThemeChange(Theme); // 系统主题改变
    };
} // fancy
#endif //SYSTEMTHEMEADAPTER_H
