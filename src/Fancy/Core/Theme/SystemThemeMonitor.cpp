//
// Created by TX on 2025/8/1.
//

#include "SystemThemeMonitor.h"

#include <QApplication>

#include "ThemeModeController.h"
#include "Core/Defs.h"

namespace fancy
{
    SystemThemeMonitor::SystemThemeMonitor(QObject* parent) :
        QThread{parent}
    {
        // 退出事件与对象同寿命，即使线程尚未创建注册表事件也能安全请求停止
        _hEvent_array[2] = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
    }

    SystemThemeMonitor::~SystemThemeMonitor()
    {
        stop();
        wait();
        if (_hEvent_array[2])
        {
            ::CloseHandle(_hEvent_array[2]);
            _hEvent_array[2] = nullptr;
        }
    }

    void SystemThemeMonitor::stop() const
    {
        ::SetEvent(this->_hEvent_array[2]);
    }

    SystemThemeMonitor& SystemThemeMonitor::monitor()
    {
        static auto* monitor = new SystemThemeMonitor(qApp);
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
        ::RegQueryValueEx(
            reg,
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
        {
            ::ResetEvent(_hEvent_array[2]);
            QThread::start();
        }
    }

    void SystemThemeMonitor::run()
    {
        // 打开深浅主题注册表项
        ::RegOpenKeyEx(HKEY_CURRENT_USER,THEME_REGEDIT_PATH, 0,KEY_NOTIFY | KEY_READ, &this->_regs[0]);

        //打开主题配色注册表
        ::RegOpenKeyEx(HKEY_CURRENT_USER,ACCENT_PALETTE_REGEDIT_PATH, 0,KEY_NOTIFY | KEY_READ, &this->_regs[1]);

        // 创建事件对象
        this->_hEvent_array[0] = ::CreateEvent(nullptr, TRUE, FALSE, nullptr); //themeChange
        this->_hEvent_array[1] = ::CreateEvent(nullptr, TRUE, FALSE, nullptr); //accentColorChange

        // 获取深浅主题改变前注册表项值
        DWORD dw_size = sizeof(DWORD);
        DWORD last_theme;
        DWORD current_theme;
        ::RegQueryValueEx(this->_regs[0],THEME_REGEDIT_KEY, nullptr, nullptr, reinterpret_cast<LPBYTE>(&last_theme), &dw_size);

        // 获取系统配色改变前的注册表项值
        const auto color_analysis = [](BYTE data[])-> QMap<SysAccentPalette, QColor> {
            QMap<SysAccentPalette, QColor> colors;
            for (int i = 0; i < 7; ++i) // 解析前 7 个颜色（每 4 字节一个颜色，RGBA 顺序） int alpha = data[i * 4 + 3]
                colors[static_cast<SysAccentPalette>(i)] = {data[i * 4 + 0], data[i * 4 + 1], data[i * 4 + 2], 255};
            return colors;
        };
        BYTE accent_colors_data[32];
        DWORD colors_size = sizeof(accent_colors_data);
        RegQueryValueEx(this->_regs[1],ACCENT_PALETTE_KEY, nullptr, nullptr, accent_colors_data, &colors_size);
        QMap<SysAccentPalette, QColor> last_accent_colors = color_analysis(accent_colors_data);

        // 两路通知各注册一次，之后仅重新注册已经触发的那一路
        ::RegNotifyChangeKeyValue(this->_regs[0], TRUE, REG_NOTIFY_CHANGE_LAST_SET, this->_hEvent_array[0], TRUE);
        ::RegNotifyChangeKeyValue(this->_regs[1], TRUE, REG_NOTIFY_CHANGE_LAST_SET, this->_hEvent_array[1], TRUE);

        while (true)
        {
            // 阻塞等待
            const DWORD dwWait = ::WaitForMultipleObjects(3, this->_hEvent_array, FALSE, INFINITE);

            // 深浅主题变化
            if (dwWait == WAIT_OBJECT_0)
            {
                // 获取当前值
                ::RegQueryValueEx(this->_regs[0],THEME_REGEDIT_KEY, nullptr, nullptr, reinterpret_cast<LPBYTE>(&current_theme), &dw_size);
                if (last_theme != current_theme)
                {
                    if (current_theme)
                        emit this->systemThemeChanged(Theme::Light);
                    else
                        emit this->systemThemeChanged(Theme::Dark);
                    last_theme = current_theme; // 0为深色，1为浅色
                }
                ::ResetEvent(this->_hEvent_array[0]); // 重置事件对象无事件
                ::RegNotifyChangeKeyValue(this->_regs[0], TRUE, REG_NOTIFY_CHANGE_LAST_SET, this->_hEvent_array[0], TRUE);
                continue;
            }

            // 主题配色变化
            if (dwWait == WAIT_OBJECT_0 + 1)
            {
                RegQueryValueEx(this->_regs[1],ACCENT_PALETTE_KEY, nullptr, nullptr, accent_colors_data, &colors_size);
                QMap<SysAccentPalette, QColor> current_accent_colors = color_analysis(accent_colors_data);
                if (last_accent_colors != current_accent_colors)
                {
                    emit systemAccentColorsChanged(current_accent_colors);
                    last_accent_colors = current_accent_colors;
                }
                ::ResetEvent(this->_hEvent_array[1]);
                ::RegNotifyChangeKeyValue(this->_regs[1], TRUE, REG_NOTIFY_CHANGE_LAST_SET, this->_hEvent_array[1], TRUE);
                continue;
            }

            // 退出线程
            if (dwWait == WAIT_OBJECT_0 + 2)
            {
                // 只释放本次运行的通知事件；退出事件保留到析构，以支持停止后重新启动
                for (int i = 0; i < 2; ++i)
                {
                    auto& hEvent = this->_hEvent_array[i];
                    if (hEvent)
                    {
                        ::CloseHandle(hEvent);
                        hEvent = nullptr;
                    }
                }
                for (auto& reg : this->_regs)
                {
                    if (reg)
                    {
                        ::RegCloseKey(reg);
                        reg = nullptr;
                    }
                }
                return;
            }
        }
    }
} // fancy
