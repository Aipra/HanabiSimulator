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
set(CMAKE_CXX_STANDARD 17)

# 全局包含头文件
include_directories(${PROJECT_SOURCE_DIR}/third_party/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui/backends)
include_directories(${PROJECT_SOURCE_DIR}/third_party/sol2/include)
include_directories(${PROJECT_SOURCE_DIR}/Kernel)
include_directories(${PROJECT_SOURCE_DIR}/UI)

set(SOURCE
    Kernel/Bullet.cpp
    Kernel/Player.cpp
    Kernel/Enemy.cpp
    Kernel/DanmakuInterpreter.cpp
    Kernel/SpellCard.cpp
    Kernel/Game.cpp
)

if(APPLE)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
    find_package(OpenGL REQUIRED)
    find_package(Lua REQUIRED)
    include_directories(/opt/homebrew/include)
    add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw OpenGL::GL ${LUA_LIBRARIES})
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
elseif(WIN32)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
    add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)
elseif(UNIX)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glad)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/glfw)
    add_executable(${PROJECT_NAME} ${SOURCE} main.cpp)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIRS})
endif()

# 设置输出路径
if(WIN32)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/win64/)
elseif(APPLE)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/macos/)
else()
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/linux/)
endif()

target_sources(HanabiSimulator PRIVATE
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl2.cpp
)

find_package(OpenGL REQUIRED)
target_link_libraries(HanabiSimulator PRIVATE OpenGL::GL)

add_custom_command(TARGET HanabiSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Scripts
        $<TARGET_FILE_DIR:HanabiSimulator>/Scripts
)
