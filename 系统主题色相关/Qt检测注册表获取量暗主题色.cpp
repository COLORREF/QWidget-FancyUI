#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
    if(settings.value("AppsUseLightTheme")==0)
    {
        // 深色主题
        qDebug()<<"深色主题";
    }
    else
    {
        // 浅色主题
        qDebug()<<"浅色主题";
    }
#endif