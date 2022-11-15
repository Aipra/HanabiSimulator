# cmake 最低版本
cmake_minimum_required(VERSION 3.23)
# 设置项目参数
project(HanabiSimulator
        DESCRIPTION "HanabiSimulator project success!"
        LANGUAGES CXX)
# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
# 包含头文件
set(KERNEL_DIR ${PROJECT_SOURCE_DIR}/Kernel)
set(UI_DIR ${PROJECT_SOURCE_DIR}/UI)
include_directories(${KERNEL_DIR})
include_directories(${UI_DIR})
set(SOURCE
        Kernel/Logical/logic.cpp
        UI/Window/window.cpp)

# 设置条件控制语句
if(WIN32)
    # 添加链接库头文件
    include_directories(${PROJECT_SOURCE_DIR}/3rd/include)
    # 添加第三方库的源码文件
    set(REDSRC
            3rd/src/glad.cpp)
    # 包含动态链接库目录x64
    link_directories(${PROJECT_SOURCE_DIR}/3rd/lib/x64/) #添加动态连接库的路径
    # 添加可执行文件
    add_executable(HanabiSimulator ${REDSRC} ${SOURCE} main.cpp)
    # 添加动态链接库x64
    target_link_libraries(HanabiSimulator PUBLIC glfw3 glfw3dll)
else()
    # 在linux系统下
endif()
# 输出可执行文件到其他文件夹下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/x64/)



