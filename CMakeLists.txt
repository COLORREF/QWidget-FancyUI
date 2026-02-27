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

# 定义库的导出宏
set(LIBRARY_NAME "QFancyUI")
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# 定义安装路径
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation prefix")

# ============================================
# 1. 创建动态库
# ============================================

# 收集库的源文件（不包括示例）
file(GLOB_RECURSE FANCYUI_SOURCES
    "${PROJECT_SOURCE_DIR}/src/Fancy/*.h"
    "${PROJECT_SOURCE_DIR}/src/Fancy/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/resource/*.qrc"
)

# 创建动态库
qt_add_library(${LIBRARY_NAME} SHARED
    ${FANCYUI_SOURCES}
)

# 设置库版本
set_target_properties(${LIBRARY_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 3
    OUTPUT_NAME "QFancyUI"
    DEBUG_POSTFIX "d"
)

# 库的头文件包含路径
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Fancy>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/include/magic_enum>
    $<INSTALL_INTERFACE:include>
)

# 链接 Qt 库
target_link_libraries(${LIBRARY_NAME} PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Xml
    Qt6::Svg
    Qt6::SvgWidgets
)

# Windows 特定链接
if (WIN32)
    target_link_libraries(${LIBRARY_NAME} PRIVATE
        user32.lib
        dwmapi.lib
        advapi32.lib
    )
endif()

# ============================================
# 2. 创建示例程序（可选）
# ============================================
option(BUILD_EXAMPLES "Build example programs" ON)

if (BUILD_EXAMPLES)
    # 收集示例源文件
    file(GLOB_RECURSE EXAMPLE_SOURCES
        "${PROJECT_SOURCE_DIR}/Example/*.h"
        "${PROJECT_SOURCE_DIR}/Example/*.cpp"
        "${PROJECT_SOURCE_DIR}/Example/*.ui"
        "${PROJECT_SOURCE_DIR}/Example/Example_Additional_Code/*.h"
        "${PROJECT_SOURCE_DIR}/Example/Example_Additional_Code/*.cpp"
        "${PROJECT_SOURCE_DIR}/Example/Example_Resource/*.qrc"
        "${PROJECT_SOURCE_DIR}/Example/Main_Program_Page_Code/*.h"
        "${PROJECT_SOURCE_DIR}/Example/Main_Program_Page_Code/*.cpp"
        "${PROJECT_SOURCE_DIR}/Example/Main_Program_Page_Code/*.ui"
    )

    # 创建示例程序
    add_executable(${PROJECT_NAME}_Example
        ${EXAMPLE_SOURCES}
    )

    # 链接到我们创建的库
    target_link_libraries(${PROJECT_NAME}_Example PRIVATE
        ${LIBRARY_NAME}
    )

    # 示例程序的头文件路径
    target_include_directories(${PROJECT_NAME}_Example PRIVATE
        ${PROJECT_SOURCE_DIR}/Example
        ${PROJECT_SOURCE_DIR}/Example/Example_Additional_Code
        ${PROJECT_SOURCE_DIR}/Example/Main_Program_Page_Code
    )

    # 设置可执行文件输出目录
    set_target_properties(${PROJECT_NAME}_Example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()

# ============================================
# 3. 安装配置
# ============================================

# 安装库文件
install(TARGETS ${LIBRARY_NAME}
    EXPORT ${LIBRARY_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY src/Fancy/
    DESTINATION include/${LIBRARY_NAME}
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*.cpp" EXCLUDE
)

install(DIRECTORY src/include/
    DESTINATION include/${LIBRARY_NAME}
    FILES_MATCHING PATTERN "*.h"
)

# 安装 CMake 配置文件
install(EXPORT ${LIBRARY_NAME}Targets
    FILE ${LIBRARY_NAME}Targets.cmake
    NAMESPACE ${LIBRARY_NAME}::
    DESTINATION lib/cmake/${LIBRARY_NAME}
)

# 创建配置文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${LIBRARY_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake
    @ONLY
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake
    DESTINATION lib/cmake/${LIBRARY_NAME}
)

# ============================================
# 4. 调试信息输出（可选）
# ============================================
# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#     message(STATUS "Building ${LIBRARY_NAME} as shared library")
#     message(STATUS "Qt version: ${Qt6_VERSION}")
#     message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
#     message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
# endif()
