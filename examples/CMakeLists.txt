cmake_minimum_required(VERSION 3.16)

find_package(Threads REQUIRED)

add_executable(simple_example simple.cpp)
target_link_libraries(simple_example PRIVATE smem)
target_link_libraries(simple_example PRIVATE ${CMAKE_THREAD_LIBS_INIT})

add_executable(ipc_server ipc-server.cpp)
target_link_libraries(ipc_server PRIVATE smem)

add_executable(ipc_client ipc-client.cpp)
target_link_libraries(ipc_client PRIVATE smem)
