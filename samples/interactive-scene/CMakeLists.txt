cmake_minimum_required(VERSION 3.27)

add_executable(interactive-scene main.cpp)
target_link_libraries(interactive-scene PRIVATE pbrlib)