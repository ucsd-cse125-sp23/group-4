cmake_minimum_required(VERSION 3.26)

project(tagguys VERSION 1.0
  DESCRIPTION "Real-time Multiplayer Tag")

add_library(core
  src/core/lib.cpp include/core/lib.hpp)
target_include_directories(core PUBLIC include)

add_library(network
  src/network/lib.cpp include/network/lib.hpp)
target_include_directories(network PUBLIC include)

add_executable(client src/client/main.cpp)
target_link_libraries(client PRIVATE core network)

add_executable(server src/server/main.cpp)
target_link_libraries(server PRIVATE core network)
