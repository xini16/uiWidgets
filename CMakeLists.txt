cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "/usr/bin/clang-18")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-18")
project(Box)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1")


find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(ui src/main.cpp src/ResourceViewUI.cpp src/ResourceManager.cpp src/Resource.cpp src/MenuBarUI.cpp src/ResourceList.cpp)

target_link_libraries(ui Qt6::Widgets)
