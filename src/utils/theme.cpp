#ifdef _WIN32
#include "theme.h"
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
    if (Theme::type != themeType)
    {
        Theme::type = themeType;
        emit Theme::themeObject() -> themeChange(Theme::type);
    }
}

void Theme::toggleLight()
{
    Theme::is_follow_system = false;
    Theme::setTheme(Theme::Type::LIGHT);
}

void Theme::toggleDark()
{
    Theme::is_follow_system = false;
    Theme::setTheme(Theme::Type::DARK);
}

void Theme::followSystem()
{
    if (!Theme::is_follow_system)
    {
        Theme::is_follow_system = true;
        QSettings settings(FULL_THEME_REGEDIT_PATH, QSettings::NativeFormat);
        if (settings.value("AppsUseLightTheme") == 0) // 深色主题
            Theme::setTheme(Theme::Type::DARK);
        else // 浅色主题
            Theme::setTheme(Theme::Type::LIGHT);
    }
}

Theme::Type Theme::themeType()
{
    return Theme::type;
}

Theme *Theme::themeObject()
{
    static Theme theme;
    return &theme;
}

bool Theme::isLigth()
{
    return Theme::type == Theme::Type::LIGHT;
}

bool Theme::isDark()
{
    return Theme::type == Theme::Type::DARK;
}

void Theme::toggleTheme()
{
    if (Theme::type == Theme::Type::LIGHT)
        Theme::setTheme(Theme::Type::DARK);
    else
        Theme::setTheme(Theme::Type::LIGHT);
}

bool Theme::isFollowSystem()
{
    return Theme::is_follow_system;
}

Theme::Type Theme::type = Type::LIGHT;
bool Theme::is_follow_system = false;

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
