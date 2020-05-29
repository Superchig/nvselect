cmake_minimum_required(VERSION 3.5)

project(nvselect LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nvselect main.cpp util.h util.cpp)

find_package(Curses REQUIRED)
target_include_directories(nvselect ${CURSES_INCLUDE_DIRECTORIES} PRIVATE)
target_link_libraries(nvselect ${CURSES_LIBRARIES})