# 收集 FancyUI 源码和 Qt 资源文件
file(GLOB_RECURSE FANCYUI_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/Fancy/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/Fancy/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/resource/*.qrc"
)

# 将 源码和资源直接加入示例程序
target_sources(${PROJECT_NAME} PRIVATE ${FANCYUI_SOURCES})

# 设置 FancyUI 源码及 magic_enum 的头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Fancy"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/magic_enum"
)

# 链接 Qt 通用模块
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Svg
)

# Qt 6 需要额外链接独立的 SvgWidgets 模块
if (QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::SvgWidgets)
endif ()

# 链接 Windows 平台所需的系统库
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE user32 dwmapi advapi32)
endif ()
