#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#define F_ACCESS_PROPERTY(var_access, type, name) \
    var_access : type _##name;

#define F_ACCESS_GETSET(type, name, Name, getter_access, setter_access, setter_expand) \
    getter_access : const type &name() const { return this->_##name; }                 \
    setter_access:                                                                     \
    void set##Name(const type &name)                                                   \
    {                                                                                  \
        if (name == this->_##name)                                                     \
        {                                                                              \
            return;                                                                    \
        }                                                                              \
        this->_##name = name;                                                          \
        setter_expand                                                                  \
    }

#define F_ACCESS_PORINTER_GET(pointer_type, name, getter_access) \
    getter_access : pointer_type name() const { return this->_##name; }

#define F_PUBLIC_PROPERTY(type, name) F_ACCESS_PROPERTY(public, type, name)
#define F_PROTECTED_PROPERTY(type, name) F_ACCESS_PROPERTY(protected, type, name)
#define F_PRIVATE_PROPERTY(type, name) F_ACCESS_PROPERTY(private, type, name)

#define F_PUBLIC_GETSET(type, name, Name, setter_expand) F_ACCESS_GETSET(type, name, Name, public, public, setter_expand)
#define F_PROTECTED_GETSET(type, name, Name, setter_expand) F_ACCESS_GETSET(type, name, Name, protected, protected, setter_expand)
#define F_PRIVATE_GETSET(type, name, Name, setter_expand) F_ACCESS_GETSET(type, name, Name, private, private, setter_expand)

#define F_PUBLIC_PORINTER_GET(pointer_type, name) F_ACCESS_PORINTER_GET(pointer_type, name, public)
#define F_PROTECTED_PORINTER_GET(pointer_type, name) F_ACCESS_PORINTER_GET(pointer_type, name, protected)
#define F_PRIVATE_PORINTER_GET(pointer_type, name) F_ACCESS_PORINTER_GET(pointer_type, name, private)

#define F_ACCESS_PROPERTY_GETSET(var_access, type, name, Name, getter_access, setter_access, setter_expand) \
    F_ACCESS_PROPERTY(var_access, type, name)                                                               \
    F_ACCESS_GETSET(type, name, Name, getter_access, setter_access, setter_expand)

// 快速创建私有属性和公共设置获取函数（指针类型无设置函数）
#define F_PRIVATE_PROPERTY_PUBLIC_GETSET(type, name, Name, setter_expand) \
    F_ACCESS_PROPERTY_GETSET(private, type, name, Name, public, public, setter_expand)
#define F_PRIVATE_POINTER_PUBLIC_GET(pointer_type, name) \
    F_PRIVATE_PROPERTY(pointer_type, name)               \
    F_PUBLIC_PORINTER_GET(pointer_type, name)

// 快速创建保护属性和公共设置获取函数（指针类型无设置函数）
#define F_PROTECTED_PROPERTY_PUBLIC_GETSET(type, name, Name, setter_expand) \
    F_ACCESS_PROPERTY_GETSET(protected, type, name, Name, public, public, setter_expand)
#define F_PROTECTED_POINTER_PUBLIC_GET(pointer_type, name) \
    F_PROTECTED_PROPERTY(pointer_type, name)               \
    F_PUBLIC_PORINTER_GET(pointer_type, name)

// 判断颜色是否属于亮色
#include <QColor>
#define isColorLight(q_color) (((5 * q_color.green()) + (2 * q_color.red()) + q_color.blue()) > (8 * 128))

#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QEnterEvent>
#include <QResizeEvent>
#include <QByteArray>
#define F_LEAVEEVENT \
    protected:virtual void leaveEvent(QEvent *event) override
#define F_DEFINITION_LEAVEEVENT(CLASS) \
    void CLASS::leaveEvent(QEvent *event)
#define F_MOUSEPRESSEVENT \
    protected:virtual void mousePressEvent(QMouseEvent *event) override
#define F_DEFINITION_MOUSEPRESSEVENT(CLASS) \
    void CLASS::mousePressEvent(QMouseEvent *event)
#define F_MOUSERELEASEEVENT \
    protected:virtual void mouseReleaseEvent(QMouseEvent *event) override
#define F_DEFINITION_MOUSERELEASEEVENT(CLASS) \
    void CLASS::mouseReleaseEvent(QMouseEvent *event)
#define F_MOUSEMOVEEVENT \
    protected:virtual void mouseMoveEvent(QMouseEvent *event) override
#define F_DEFINITION_MOUSEMOVEEVENT(CLASS) \
    void CLASS::mouseMoveEvent(QMouseEvent *event)
#define F_PAINTEVENT \
    protected:virtual void paintEvent(QPaintEvent *event) override
#define F_DEFINITION_PAINTEVENT(CLASS) \
    void CLASS::paintEvent(QPaintEvent *event)
#define F_CHANGEEVENT\
    protected:virtual void changeEvent(QEvent *event) override
#define F_DEFINITION_CHANEGEVENT(CLASS) \
    void CLASS::changeEvent(QEvent *event)
#define F_RESIZEEVENT\
    protected:virtual void resizeEvent(QResizeEvent *event) override
#define F_DEFINITION_RESIZEEVENT(CLASS)\
void CLASS::resizeEvent(QResizeEvent *event)

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
#define F_ENTEREVENT \
    protected:virtual void enterEvent(QEnterEvent *event) override
#define F_DEFINITION_ENTEREVENT(CLASS) \
    void CLASS::enterEvent(QEnterEvent *event)
#define F_NATIVEEVENT \
    protected:virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override
#define F_DEFINITION_NATIVEEVENT(CLASS) \
    bool CLASS::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
#define F_ENTEREVENT \
    protected:virtual void enterEvent(QEvent *event) override
#define F_DEFINITION_ENTEREVENT(CLASS, function_body) \
    void CLASS::enterEvent(QEvent *event) { function_body }
#define F_NATIVEEVENT \
    protected:virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#define F_DEFINITION_NATIVEEVENT(CLASS) \
    bool CLASS::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif


#include <QStandardPaths>
#include <QCoreApplication>
// 以下所有注释均针对Windows系统而言
// 对于中文语言的Windows系统， C:/用户/... 和  C:/User/... 是相同的
// 对于以下的注释内容，C:/用户/.../   用户/  后面的三个点均指代电脑的用户名称
#define desktopPath QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)           // 桌面路径
#define documentPath QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)        // 文档路径
#define fontPath QStandardPaths::writableLocation(QStandardPaths::FontsLocation)                // 字体文件路径
#define applicationsPath QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) // 开始菜单程序路径
#define musicPath QStandardPaths::writableLocation(QStandardPaths::MusicLocation)               // 音乐路径
#define moviesPath QStandardPaths::writableLocation(QStandardPaths::MoviesLocation)             // 视频路径
#define picturesPath QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)         // 图片路径
#define tempPath QStandardPaths::writableLocation(QStandardPaths::TempLocation)                 // 临时文件路径
#define homePath QStandardPaths::writableLocation(QStandardPaths::HomeLocation)                 // 主目录路径
#define cachePath QStandardPaths::writableLocation(QStandardPaths::CacheLocation)               // 本应用程序的缓存路径（C:/用户/.../AppAppData/Local/ + 本项目名称 + /cache ，该路径可能是不存在的）
#define genericCachePath QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation) // 系统通用缓存目录路径（参考： C:/用户/.../AppData/Local/cache）
#define genericDataPath QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)   // 系统通用数据目录路径 (参考： C:/用户/.../AppData/Local)
// #define runTimePath QStandardPaths::writableLocation(QStandardPaths::RuntimeLocation)//意义不明，区别于最后一个 runPath 参考：C:/Users/... 为了不与最后一项产生歧义和误解，这里将之注释掉
#define configPath QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)               // 似乎与appLocalDataPath是相同的
#define downloadPath QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)           // 下载路径
#define genericConfigPath QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) // 通用配置目录路径 似乎与genericDataPath是相同的

#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))                                                   // Qt5.4及以上生效
#define appLocalDataPath QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) // 本程序的配置文件、数据文件等默认的系统目录路径（返回值是默认的系统应用数据路径加上本项目名称，即大部分软件的默认安装路径 C:/用户/.../AppData/Local/ + 本项目名称，该路径可能是不存在的）
#define appDataPath QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)           // 本应用程序数据路径 （参考：C:/用户/.../AppData/Roaming/ + 本项目名称 ，大部分情况下也是存储一些配置文件或者缓存文件）
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 5, 0))                                             // Qt5.5及以上生效
#define appConfigPath QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) // 本应用程序配置目录路径，似乎与appLocalDataPath是相同的 参考：C:/用户/.../AppData/Local/ + 本项目名称
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))                                                 // Qt6.4及以上生效
#define publicSharePath QStandardPaths::writableLocation(QStandardPaths::PublicShareLocation) // 系统公共目录 参考：C:/Users/Public 或C:/用户/公用
#define templatesPath QStandardPaths::writableLocation(QStandardPaths::TemplatesLocation)     // 返回可存储特定于用户的模板文件的目录位置。这是一个泛型值。如果系统没有模板位置的概念，返回的路径可能为空。参考：C:/用户/.../AppData/Roaming/Microsoft/Windows/Templates
#endif

#define runPath QCoreApplication::applicationDirPath() // 本程序运行时的路径，即调用的exe运行时的路径，该路径是动态的

#endif // PRECOMPILE_H
