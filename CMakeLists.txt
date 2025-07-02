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
    # 添加子模块glfw，并构建为静态链接库
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
    

    # 添加可执行文件
    add_executable(${PROJECT_NAME} ${REDSRC} ${SOURCE} main.cpp)
    # 添加动态链接库x64
    # 5. 链接 glfw 库
    target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)

else()
    # 在linux系统下
endif()
# 输出可执行文件到其他文件夹下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/x64/)
