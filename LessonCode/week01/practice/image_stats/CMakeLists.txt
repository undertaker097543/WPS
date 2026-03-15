cmake_minimum_required(VERSION 3.10)
project(image_stats)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Gui Core REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Qt5::Gui Qt5::Core)