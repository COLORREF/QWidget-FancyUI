//
// Created by TX on 2025/8/1.
//

#include "ThemeModeController.h"
#include "Defs.h"
#include "SystemThemeMonitor.h"

namespace fancy
{
    ThemeModeController::ThemeModeController(QObject *parent) :
        QObject(parent)
    {
        _system_theme = SystemThemeMonitor::SystemTheme();
        _app_theme = _system_theme;
        SystemThemeMonitor::monitor().start();
        connect(&SystemThemeMonitor::monitor(), &SystemThemeMonitor::systemThemeChanged, this, &ThemeModeController::onSystemThemeChanged);
    }

    ThemeModeController::~ThemeModeController()
    {
        if (SystemThemeMonitor::monitor().isRunning())
            SystemThemeMonitor::monitor().stop();
        SystemThemeMonitor::monitor().wait();
    }

    void ThemeModeController::setAppTheme(const Theme theme)
    {
        _is_follow_system = false;
        _app_theme = theme;
        emit this->appThemeChange(theme);
    }

    void ThemeModeController::toggleAppLight()
    {
        if (_app_theme == Theme::Dark)
            setAppTheme(Theme::Light);
    }

    void ThemeModeController::toggleAppDark()
    {
        if (_app_theme == Theme::Light)
            setAppTheme(Theme::Dark);
    }

    void ThemeModeController::followSystem()
    {
        if (!_is_follow_system)
        {
            _is_follow_system = true;
            _app_theme = _system_theme;
            emit this->appThemeChange(_app_theme);
        }
    }

    Theme ThemeModeController::appTheme() const
    {
        return _app_theme;
    }

    Theme ThemeModeController::systemTheme() const
    {
        return _system_theme;
    }

    ThemeModeController &ThemeModeController::controller()
    {
        static auto *theme = new ThemeModeController(qApp);
        return *theme;
    }

    bool ThemeModeController::isAppLight() const
    {
        return _app_theme == Theme::Light;
    }

    bool ThemeModeController::isAppDark() const
    {
        return _app_theme == Theme::Dark;
    }

    void ThemeModeController::toggleAppTheme()
    {
        if (_app_theme == Theme::Light)
            setAppTheme(Theme::Dark);
        else
            setAppTheme(Theme::Light);
    }

    void ThemeModeController::onSystemThemeChanged(const Theme theme)
    {
        _system_theme = theme;
        emit this->systemThemeChange(theme);
        if (_is_follow_system)
        {
            _app_theme = _system_theme;
            emit this->appThemeChange(_app_theme);
        }
    }

    bool ThemeModeController::isFollowSystem() const
    {
        return _is_follow_system;
    }
} // fancy
