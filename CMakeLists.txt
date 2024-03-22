cmake_minimum_required(VERSION 3.27)
project(Snake C)

set(CMAKE_C_STANDARD 11)

if (LINUX)
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
endif()

add_executable(Snake main.c)

if (LINUX)
    target_link_libraries(Snake ${CURSES_LIBRARIES})
endif()
