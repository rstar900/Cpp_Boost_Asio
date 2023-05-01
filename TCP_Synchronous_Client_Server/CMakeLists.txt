cmake_minimum_required(VERSION 3.15)

project(TCP_Synchronous_Client_Server VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_executable(tcp-sync-server src/server.cpp)
add_executable(tcp-sync-client src/client.cpp)
target_link_libraries(tcp-sync-server ${Boost_LIBRARIES})
target_link_libraries(tcp-sync-client ${Boost_LIBRARIES})