cmake_minimum_required(VERSION 3.27)
project(Snake C)

set(CMAKE_C_STANDARD 11)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(Snake main.c)
target_link_libraries(Snake ${CURSES_LIBRARIES})
