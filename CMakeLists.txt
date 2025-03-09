cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(Box)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(ui src/main.cpp src/ResourceViewUI.cpp src/ResourceManager.cpp src/Resource.cpp src/MenuBarUI.cpp src/ResourceList.cpp)

target_link_libraries(ui Qt6::Widgets)
