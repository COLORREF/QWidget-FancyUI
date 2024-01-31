#pragma comment(lib, "Advapi32.lib")

#include <stdio.h>
#include <windows.h>

int main(int argc, const char *argv[])
{
    HKEY hKey;
    RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_NOTIFY | KEY_READ, &hKey);
    DWORD dwSize = sizeof(DWORD);
    DWORD queryValueLast;
    DWORD queryValue;
    RegQueryValueExA(hKey, "AppsUseLightTheme", NULL, NULL, (LPBYTE)&queryValueLast, &dwSize);
    while(true)
    {
        RegNotifyChangeKeyValue(hKey, TRUE, REG_NOTIFY_CHANGE_LAST_SET, NULL, FALSE);
        RegQueryValueExA(hKey, "AppsUseLightTheme", NULL, NULL, (LPBYTE)&queryValue, &dwSize);
        if (queryValueLast != queryValue)
        {
            queryValueLast = queryValue;
            puts(queryValue ? "Light" : "Dark");
        }
    }
    return 0;
}