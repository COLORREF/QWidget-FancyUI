cmake_minimum_required(VERSION 3.10)
project(QWidget_FancyUI VERSION 3.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

unset(ENV{PATH})

# 在此设置你的Qt路径  Set your Qt path here
if (MSVC)
    set(CMAKE_PREFIX_PATH "E:/Qt/6.7.2/msvc2019_64") # MSVC
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_PREFIX_PATH "E:/Qt/6.7.2/mingw_64")    # MinGW
endif ()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Xml Svg SvgWidgets REQUIRED)

file(GLOB_RECURSE FANCYUI_SOURCES
        "${PROJECT_SOURCE_DIR}/src/Fancy/*.h"
        "${PROJECT_SOURCE_DIR}/src/Fancy/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/Fancy/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/resource/*.qrc"
)

file(GLOB_RECURSE EXAMPLE_SOURCES
        "${PROJECT_SOURCE_DIR}/Example/Example_Additional_Code/*.h"
        "${PROJECT_SOURCE_DIR}/Example/Example_Additional_Code/*.cpp"
        "${PROJECT_SOURCE_DIR}/Example/Example_Resource/*.qrc"
        "${PROJECT_SOURCE_DIR}/Example/*.h"
        "${PROJECT_SOURCE_DIR}/Example/*.cpp"
        "${PROJECT_SOURCE_DIR}/Example/*.ui"
)

add_executable(${PROJECT_NAME}
        ${FANCYUI_SOURCES}
        ${EXAMPLE_SOURCES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Svg
        Qt${QT_VERSION_MAJOR}::SvgWidgets
)

set(FANCY_SRC_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/src/Fancy
        ${PROJECT_SOURCE_DIR}/src/include/magic_enum
)

set(EXAMPLE_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/Example
        ${PROJECT_SOURCE_DIR}/Example/Example_Additional_Code
)

include_directories(${FANCY_SRC_INCLUDE_DIRS})
include_directories(${EXAMPLE_INCLUDE_DIRS})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            user32.lib
            dwmapi.lib
            advapi32.lib
    )
endif ()

#message(STATUS "CMake version: ${CMAKE_VERSION}")                   # CMake版本
#message(STATUS "CMake generator: ${CMAKE_GENERATOR}")               # 使用的生成器
#message(STATUS "Qt version: ${Qt6_VERSION}")                        # Qt版本
#message(STATUS "Qt Core module version: ${Qt6Core_VERSION}")        # QtCore模块版本
#message(STATUS "Qt Core include dirs: ${Qt6Core_INCLUDE_DIRS}")     # QtCore头文件路径
#message(STATUS "Qt AutoMOC enabled: ${CMAKE_AUTOMOC}")              # 元对象编译器状态
#message(STATUS "Qt AutoRCC enabled: ${CMAKE_AUTORCC}")              # 资源编译器状态
#message(STATUS "Qt AutoUIC enabled: ${CMAKE_AUTOUIC}")              # UIC编译器状态
#message(STATUS "compiler: ${CMAKE_CXX_COMPILER}")                   # 编译器路径
#message(STATUS "compiler ID: ${CMAKE_CXX_COMPILER_ID}")             # 编译器厂商(MSVC/GNU/Clang等)
#message(STATUS "compiler version: ${CMAKE_CXX_COMPILER_VERSION}")   # 编译器版本号
#message(STATUS "compiler flags: ${CMAKE_CXX_FLAGS}")                # 全局编译选项
#message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")               # C++标准
#message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")                   # 编译类型(Release/Debug等)
#message(STATUS "Project name: ${PROJECT_NAME}")                     # 项目名称
#message(STATUS "Project version: ${PROJECT_VERSION}")               # 项目版本号
#message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")             # 源代码根目录
#message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")              # 构建目录
#message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")           # 安装时的基础路径


