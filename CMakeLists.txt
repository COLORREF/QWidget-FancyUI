cmake_minimum_required(VERSION 3.15)
project(QWidget_FancyUI VERSION 3.0.0 LANGUAGES CXX)

# 启用 C++17 和 Qt 的自动代码生成
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 在此设置你的 Qt 路径，并按实际按需开启静态编译选项
# 已支持 Qt5 可设置 Qt5 路径 推荐 5.15 及以上版本
if (MSVC)
    set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")
    # set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # 仅当 Qt 为静态库且为 /MT 时开启，/MD 保持注释
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_PREFIX_PATH "C:/Qt/6.11.1/mingw_64")
    # add_link_options(-static -static-libgcc -static-libstdc++) # MinGW 全静态链接时按需开启；静态 Qt 库本身不要求这些选项
endif ()

# 根据指定的 Qt 路径查找 Qt 5 或 Qt 6 及所需模块
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Xml Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Xml Svg)
if (QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS SvgWidgets)
endif ()

# 收集示例程序的源码、界面和资源
file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/Example/*.h"
        "${PROJECT_SOURCE_DIR}/Example/*.cpp"
        "${PROJECT_SOURCE_DIR}/Example/*.ui"
        "${PROJECT_SOURCE_DIR}/Example/Example_Resource/*.qrc"
)

# 创建示例程序
add_executable(${PROJECT_NAME} ${EXAMPLE_SOURCES})

# 设置示例程序的头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/Example"
        "${PROJECT_SOURCE_DIR}/Example/Example_Additional_Code"
        "${PROJECT_SOURCE_DIR}/Example/Main_Program_Page_Code"
)

# 将 FancyUI 源码及依赖加入示例程序
add_subdirectory(src)
