//
// Created by TX on 2025/8/1.
//

#ifndef SYSTEMTHEMEMONITOR_H
#define SYSTEMTHEMEMONITOR_H

#include <QApplication>
#include <QSettings>
#include <QThread>
#include <Windows.h>


namespace fancy
{
    enum class Theme;
    enum class SysAccentPalette;

    class SystemThemeMonitor final : public QThread
    {
        Q_OBJECT

        explicit SystemThemeMonitor(QObject *parent = nullptr);

        HKEY _regs[2] = {nullptr}; // 注册表项句柄
        HANDLE _hEvent_array[3] = {nullptr}; // 事件对象数组
    public:
        static SystemThemeMonitor &monitor();

        [[nodiscard]] static Theme SystemTheme();

        void start();

        void stop() const;

    protected:
        void run() override;

    signals:
        void systemThemeChanged(Theme theme);

        void systemAccentColorsChanged(QMap<SysAccentPalette, QColor> colors);
    };
} // fancy

#endif //SYSTEMTHEMEMONITOR_H
