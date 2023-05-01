cmake_minimum_required(VERSION 3.15)

project(TCP_Asynchronous_Client_Server VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_executable(tcp-async-server src/server.cpp)
add_executable(tcp-async-client src/client.cpp)
target_link_libraries(tcp-async-server ${Boost_LIBRARIES})
target_link_libraries(tcp-async-client ${Boost_LIBRARIES})