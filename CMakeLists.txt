cmake_minimum_required(VERSION 3.10)

project(MyUnix)

set(CMAKE_CXX_STANDARD 11)

include_directories(./src)
aux_source_directory(./src source_list)

add_executable(MyUnix ${source_list})