cmake_minimum_required(VERSION 3.10)

project(MyUnix)

find_package(Curses REQUIRED)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(googletest)

include_directories(./src googletest/include ${CURSES_INCLUDE_DIR})
aux_source_directory(./src source_list)

add_executable(MyUnix ${source_list})
target_link_libraries(MyUnix PRIVATE gtest ${CURSES_LIBRARIES})