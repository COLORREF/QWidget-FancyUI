#ifdef _WIN32
#include "FancyTheme.h"
#define THEME_REGEDIT_PATH "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"
#define FULL_THEME_REGEDIT_PATH "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"

Theme::Theme(QObject *parent)
    : QObject{parent}
{
    this->L = new ListeningSystemTheme(this);
    this->L->start();
}

Theme::~Theme()
{
    if (L->isRunning())
        L->stop();
    L->wait();
}

void Theme::setTheme(Type themeType)
{
    Theme::_type = themeType;
    emit Theme::themeObject() -> themeChange(themeType);
}

void Theme::toggleLight()
{
    Theme::_is_follow_system = false;
    if((bool)Theme::_type)//Theme::_type == Theme::Type::DARK
        Theme::setTheme(Theme::Type::LIGHT);
}

void Theme::toggleDark()
{
    Theme::_is_follow_system = false;
    if(!(bool)Theme::_type)//Theme::_type == Theme::Type::LIGHT
        Theme::setTheme(Theme::Type::DARK);
}

void Theme::followSystem()
{
    if (!Theme::_is_follow_system)
    {
        Theme::_is_follow_system = true;
        QSettings settings(FULL_THEME_REGEDIT_PATH, QSettings::NativeFormat);
        if (settings.value("AppsUseLightTheme") == 0) // 深色主题
        {
            if(!(bool)Theme::_type)//Theme::_type == Theme::Type::LIGHT
                Theme::setTheme(Theme::Type::DARK);
        }
        else // 浅色主题
            if((bool)Theme::_type)//Theme::_type == Theme::Type::DARK
                Theme::setTheme(Theme::Type::LIGHT);
    }
}

Theme::Type Theme::themeType()
{
    return Theme::_type;
}

Theme *Theme::themeObject()
{
    static Theme theme;
    return &theme;
}

bool Theme::isLigth()
{
    return Theme::_type == Theme::Type::LIGHT;
}

bool Theme::isDark()
{
    return Theme::_type == Theme::Type::DARK;
}

void Theme::toggleTheme()
{
    if (Theme::_type == Theme::Type::LIGHT)
        Theme::setTheme(Theme::Type::DARK);
    else
        Theme::setTheme(Theme::Type::LIGHT);
}

bool Theme::isFollowSystem()
{
    return Theme::_is_follow_system;
}

Theme::Type Theme::_type = Type::LIGHT;
bool Theme::_is_follow_system = false;

ListeningSystemTheme::ListeningSystemTheme(QObject *parent)
    : QThread::QThread{parent}
{
}

void ListeningSystemTheme::stop()
{
    ::SetEvent(this->hEventArray[1]);
}

void ListeningSystemTheme::run()
{
    // 打开注册表项
    ::RegOpenKeyExA(
        HKEY_CURRENT_USER,
        THEME_REGEDIT_PATH,
        0,
        KEY_NOTIFY | KEY_READ,
        &this->reg);

    // 创建事件对象
    this->hEventArray[0] = ::CreateEvent(NULL, TRUE, FALSE, TEXT("themeChange"));
    this->hEventArray[1] = ::CreateEvent(NULL, TRUE, FALSE, TEXT("exit_"));

    DWORD dw_size = sizeof(DWORD);
    DWORD last_theme;
    DWORD current_theme;

    // 获取改变前注册表项值
    ::RegQueryValueEx(this->reg, TEXT("AppsUseLightTheme"), NULL, NULL, (LPBYTE)&last_theme, &dw_size);

    while (true)
    {
        // 监测注册表项值，让WaitForMultipleObjects等待此事件
        ::RegNotifyChangeKeyValue(this->reg, TRUE, REG_NOTIFY_CHANGE_LAST_SET, this->hEventArray[0], TRUE);

        // 阻塞等待
        DWORD dwWait = ::WaitForMultipleObjects(2, this->hEventArray, FALSE, INFINITE);

        if (dwWait == WAIT_OBJECT_0)
        {
            // 获取当前注册表值
            ::RegQueryValueEx(this->reg, TEXT("AppsUseLightTheme"), NULL, NULL, (LPBYTE)&current_theme, &dw_size);

            if (last_theme != current_theme)
            {
                last_theme = current_theme;
                if (Theme::isFollowSystem())
                {
                    // 0为深色，1为浅色
                    if (current_theme) // Light
                        Theme::setTheme(Theme::Type::LIGHT);
                    else // Dark
                        Theme::setTheme(Theme::Type::DARK);
                }
            }
            ::ResetEvent(this->hEventArray[0]); // 重置事件对象无事件
            continue;
        }

        if (dwWait == WAIT_OBJECT_0 + 1)
        {
            ::ResetEvent(this->hEventArray[0]);
            ::ResetEvent(this->hEventArray[1]);
            ::CloseHandle(this->hEventArray[0]);
            ::CloseHandle(this->hEventArray[1]);
            ::RegCloseKey(this->reg);
            return;
        }
    }
}
#endif
