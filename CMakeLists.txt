cmake_minimum_required(VERSION 3.16)
project(OpenCV_CPP_Quickstart LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 源码使用 UTF-8（含中文注释）。MSVC 默认按系统 ANSI 代码页（简体中文系统为 GBK）
# 解析源文件，会把 UTF-8 字节误判成 GBK 双字节字符并吞掉换行，导致大量语法错误。
# 显式声明源码字符集与执行字符集为 UTF-8。
if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(OpenCV 4 REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")

function(add_cv_example target source)
    add_executable(${target} ${source})
    target_include_directories(${target} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE ${OpenCV_LIBS})
endfunction()

# 自动收集 src/ 下所有 .cpp，每个文件编译成一个独立可执行目标（目标名 = 文件名去扩展名）。
# 这样你新增/重命名练习文件后不必改 CMakeLists，重新配置即可。
# CONFIGURE_DEPENDS 让 CMake 在构建时重新扫描目录。
file(GLOB CV_EXAMPLE_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

if(NOT CV_EXAMPLE_SOURCES)
    message(WARNING "src/ 下没有 .cpp 文件：先在 src/ 里创建并实现你的示例")
endif()

foreach(cv_source IN LISTS CV_EXAMPLE_SOURCES)
    get_filename_component(cv_target "${cv_source}" NAME_WE)
    add_cv_example(${cv_target} ${cv_source})
endforeach()
