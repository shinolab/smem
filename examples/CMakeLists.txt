cmake_minimum_required(VERSION 3.16)

find_package(Threads REQUIRED)

add_executable(simple_example simple.cpp)
target_link_libraries(simple_example PRIVATE smem)
target_link_libraries(simple_example PRIVATE ${CMAKE_THREAD_LIBS_INIT})
