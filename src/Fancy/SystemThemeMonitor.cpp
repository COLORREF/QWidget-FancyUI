//
// Created by TX on 2025/8/1.
//

#include "SystemThemeMonitor.h"

#include <QApplication>

#include "Defs.h"
#include "ThemeModeController.h"

namespace fancy
{
    SystemThemeMonitor::SystemThemeMonitor(QObject *parent) :
        QThread{parent} {}

    void SystemThemeMonitor::stop() const
    {
        ::SetEvent(this->_hEvent_array[2]);
    }


    SystemThemeMonitor &SystemThemeMonitor::monitor()
    {
        static auto *monitor = new SystemThemeMonitor(qApp);
        return *monitor;
    }

    Theme SystemThemeMonitor::SystemTheme()
    {
        HKEY reg;
        ::RegOpenKeyEx(
            HKEY_CURRENT_USER,
            THEME_REGEDIT_PATH,
            0,
            KEY_NOTIFY | KEY_READ,
            &reg
        );
        // 获取深浅主题改变前注册表项值
        DWORD dw_size = sizeof(DWORD);
        DWORD theme;
        ::RegQueryValueEx(reg,
                          THEME_REGEDIT_KEY,
                          nullptr,
                          nullptr,
                          reinterpret_cast<LPBYTE>(&theme),
                          &dw_size
        );
        RegCloseKey(reg);
        return theme ? Theme::Light : Theme::Dark;
    }

    void SystemThemeMonitor::start()
    {
        if (!this->isRunning())
            QThread::start();
    }

    void SystemThemeMonitor::run()
    {
        // 打开深浅主题注册表项
        ::RegOpenKeyEx(
            HKEY_CURRENT_USER,
            THEME_REGEDIT_PATH,
            0,
            KEY_NOTIFY | KEY_READ,
            &this->_regs[0]
        );

        //打开主题配色注册表
        ::RegOpenKeyEx(
            HKEY_CURRENT_USER,
            ACCENT_PALETTE_REGEDIT_PATH,
            0,
            KEY_NOTIFY | KEY_READ,
            &this->_regs[1]
        );

        // 创建事件对象
        this->_hEvent_array[0] = ::CreateEvent(nullptr, TRUE, FALSE, nullptr); //themeChange
        this->_hEvent_array[1] = ::CreateEvent(nullptr, TRUE, FALSE, nullptr); //accentColorChange
        this->_hEvent_array[2] = ::CreateEvent(nullptr, TRUE, FALSE, nullptr); //exit

        // 获取深浅主题改变前注册表项值
        DWORD dw_size = sizeof(DWORD);
        DWORD last_theme;
        DWORD current_theme;
        ::RegQueryValueEx(this->_regs[0],
                          THEME_REGEDIT_KEY,
                          nullptr,
                          nullptr,
                          reinterpret_cast<LPBYTE>(&last_theme),
                          &dw_size
        );

        // 获取系统配色改变前的注册表项值
        const auto color_analysis = [](BYTE data[])-> QMap<SysAccentPalette, QColor> {
            QMap<SysAccentPalette, QColor> colors;
            for (int i = 0; i < 7; ++i) // 解析前 7 个颜色（每 4 字节一个颜色，RGBA 顺序） int alpha = data[i * 4 + 3]
                colors[static_cast<SysAccentPalette>(i)] = {data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2], 255};
            return colors;
        };
        BYTE accent_colors_data[32];
        DWORD colors_size = sizeof(accent_colors_data);
        QMap<SysAccentPalette, QColor> last_accent_colors = color_analysis(accent_colors_data);
        RegQueryValueEx(this->_regs[1],
                        ACCENT_PALETTE_KEY,
                        nullptr,
                        nullptr,
                        accent_colors_data,
                        &colors_size
        );

        while (true)
        {
            // 监测注册表项值，让WaitForMultipleObjects等待此事件
            ::RegNotifyChangeKeyValue(this->_regs[0], TRUE, REG_NOTIFY_CHANGE_LAST_SET, this->_hEvent_array[0], TRUE);
            ::RegNotifyChangeKeyValue(this->_regs[1], TRUE, REG_NOTIFY_CHANGE_LAST_SET, this->_hEvent_array[1], TRUE);

            // 阻塞等待
            const DWORD dwWait = ::WaitForMultipleObjects(3, this->_hEvent_array, FALSE, INFINITE);

            // 深浅主题变化
            if (dwWait == WAIT_OBJECT_0)
            {
                // 获取当前值
                ::RegQueryValueEx(this->_regs[0],
                                  THEME_REGEDIT_KEY,
                                  nullptr,
                                  nullptr,
                                  reinterpret_cast<LPBYTE>(&current_theme),
                                  &dw_size
                );
                if (last_theme != current_theme)
                {
                    if (current_theme)
                        emit this->systemThemeChanged(Theme::Light);
                    else
                        emit this->systemThemeChanged(Theme::Dark);
                    last_theme = current_theme; // 0为深色，1为浅色
                }
                ::ResetEvent(this->_hEvent_array[0]); // 重置事件对象无事件
                continue;
            }

            // 主题配色变化
            if (dwWait == WAIT_OBJECT_0 + 1)
            {
                RegQueryValueEx(this->_regs[1],
                                ACCENT_PALETTE_KEY,
                                nullptr,
                                nullptr,
                                accent_colors_data,
                                &colors_size
                );
                QMap<SysAccentPalette, QColor> current_accent_colors = color_analysis(accent_colors_data);
                if (last_accent_colors != current_accent_colors)
                {
                    emit systemAccentColorsChanged(current_accent_colors);
                    last_accent_colors = current_accent_colors;
                }
                ::ResetEvent(this->_hEvent_array[1]);
                continue;
            }

            // 退出线程
            if (dwWait == WAIT_OBJECT_0 + 2)
            {
                for (const auto &hEvent: this->_hEvent_array)
                {
                    ::ResetEvent(hEvent);
                    ::CloseHandle(hEvent);
                }
                for (const auto &reg: this->_regs)
                    ::RegCloseKey(reg);
                return;
            }
        }
    }
} // fancy
