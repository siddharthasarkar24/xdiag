cmake_minimum_required(VERSION 3.19)

project(hello_world)

find_package(xdiag REQUIRED HINTS "/path/to/where/xdiag/should/be/install")
add_executable(main main.cpp)
target_link_libraries(main PRIVATE xdiag::xdiag)
