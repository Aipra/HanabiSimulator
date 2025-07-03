# cmake 最低版本
cmake_minimum_required(VERSION 3.23)

# 设置CMake策略以兼容旧版本的GLAD
# 使用CMAKE_POLICY_VERSION_MINIMUM来兼容旧版本
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

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
    # Windows系统下的编译方案
    # 添加子模块glfw，并构建为静态链接库
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
    
    # 添加可执行文件
    add_executable(${PROJECT_NAME} ${REDSRC} ${SOURCE} main.cpp)
    # 添加动态链接库x64
    # 5. 链接 glfw 库
    target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)

elseif(APPLE)
    # macOS系统下的编译方案
    # 添加子模块glfw，并构建为静态链接库
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
    
    # 添加可执行文件
    add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)
    
    # 链接 glfw 库和macOS系统库
    target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)
    
    # macOS特定的系统库
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})

else()
    # Linux系统下的编译方案
    # 添加子模块glfw，并构建为静态链接库
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
    
    # 添加可执行文件
    add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)
    
    # 链接 glfw 库和Linux系统库
    target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)
    
    # Linux特定的系统库
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIRS})
endif()
# 根据平台设置输出路径
if(WIN32)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/win64/)
elseif(APPLE)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/macos/)
else()
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/linux/)
endif()
